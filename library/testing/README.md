# midge Testing Harness

Header-only utilities for writing correctness tests and throughput benchmarks of midge
pipelines. Requires only `diptera.hh`, `producer.hh`, `consumer.hh`, and the C++ standard
library — nothing from sandfly, nothing project-specific.

## Quick start

```cpp
#include "midge_test_nodes.hh"
#include "midge_test_harness.hh"

using namespace midge::testing;

// All nodes MUST be heap-allocated: diptera::reset() deletes them.
auto* t_prod = new test_producer< my_data >();
auto* t_cons = new test_consumer< my_data >();

// Names are mandatory -- the default is "(unnamed node)" and add() rejects duplicates.
t_prod->set_name( "prod" );
t_cons->set_name( "cons" );

t_prod->set_n_records( 5 );
t_prod->set_fill( []( my_data* slot, std::size_t i ) {
    slot->set_value( static_cast< int >( i ) );
} );
t_cons->set_expect_records( 5 );

std::vector< int > results;
t_cons->set_extract( [&]( const my_data* rec, std::size_t ) {
    results.push_back( rec->get_value() );
} );

midge::diptera t_midge;
t_midge.add( t_prod );
t_midge.add( t_cons );
t_midge.join( "prod.out_0:cons.in_0" );

auto t_result = run_with_watchdog( t_midge, "prod:cons" );

// Check the watchdog FIRST: a timeout means the pipeline hung.
assert( ! t_result.f_timed_out );
assert( ! t_result.f_exception );
assert( results.size() == 5 );
// t_midge destructs here: reset() -> finalize() + delete on both nodes.
// results is still valid (deep copies owned by this scope).
```

## Key constraints (each has a concrete consequence)

### 1. Heap-allocate every node

`diptera::reset()` (called from `~diptera()`) calls `finalize()` then `delete` on every
node it owns. Stack-allocating a node and passing its address will double-free.

### 2. Every node needs an explicit name

`node::node()` sets `f_name` to `"(unnamed node)"`. `diptera::add()` throws on duplicate
names, so two unnamed nodes in the same pipeline immediately crash setup.

### 3. Never copy payload types

Many midge payload types hold raw owning pointers (`float*`, `complex_t*`, `U16*`) with a
destructor that frees them, and no copy constructor. Copying produces two objects sharing one
buffer — the second destructor double-frees.

The harness is designed around this:

- `set_prepare` / `set_fill` write *in place* into the slot the buffer already owns.
- `set_extract` must **deep-copy** whatever it cares about into caller-owned POD or
  containers before returning. The buffer slot is reused as soon as the extract returns.
- Never store `x_type*` beyond the extract call. Never put `x_type` in a `std::vector`.

### 4. Shutdown is via cancellation

Most production nodes loop on `while( ! is_canceled() )` and never break on `s_stop` or
`s_exit`. Once the producer exits, these nodes spin forever waiting for the next buffer slot,
and `diptera::run()` blocks in `thread::join()`.

`test_consumer` drives shutdown: after receiving `expect_records` records *and* a terminal
command (`s_stop`, `s_exit`, `s_error`), it calls `diptera::cancel()`. `diptera` then
cancels producers → transformers → consumers, exactly as production does.

### 5. A watchdog is mandatory in tests

Without it, a node that never terminates causes the test to hang forever instead of failing.
`run_with_watchdog` runs the pipeline in the calling thread and drives a watchdog in a
background thread. If the pipeline does not finish within the timeout, the watchdog calls
`cancel()` and `f_timed_out` is set to `true`.

**Always check `f_timed_out` before any other assertion** — if the pipeline hung, every
downstream assertion is meaningless noise.

Cancelling from the watchdog thread is safe: `diptera::run()` releases `f_threads_mutex`
before joining threads, so `do_cancellation()` can always acquire it.

### 6. Fixed overhead dominates short runs

`diptera::run()` sleeps 100 ms for thread spin-up. `do_cancellation()` sleeps 500 ms between
node tiers. A pipeline that finishes in microseconds still takes ~600 ms wall-clock. Set the
watchdog timeout well above that (default: 10 s).

### 7. A canceled `diptera` cannot be re-run

`diptera::run()` does not reset cancellation state. If you need multiple runs, construct a
fresh `diptera` per test — or call `reset_cancel()` after `reset()`.

### 8. Join / run string syntax

- `join("prod.out_0:cons.in_0")` — connect output 0 of `prod` to input 0 of `cons`
- `run("prod:node:cons")` — run these three nodes; `:` is the separator

The designator is `.` and the connector between the two sides of a join is `:`.

### 9. Unconnected inputs are safe only if untouched

A multi-input node (e.g. `dead_end` with four input streams) has a null internal stream
pointer for any unjoined input. Leaving a slot unjoined is safe *only* if the node's execute
loop never calls `in_stream< N >().get()` for that slot. Read the node's switch/if logic
carefully before relying on this.

### 10. Prepare vs. fill

`set_prepare` runs once per buffer slot at `initialize()` time and is useful for one-off
setup like `allocate_array`. `set_fill` runs once per emitted record and should write the
record content. Both receive the slot pointer and a sequential index; both must never copy
`x_type`.
