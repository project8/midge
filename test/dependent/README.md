# PackageBuilder Test

This directory contains the infrastructure to test the PackageBuilder framework, including the use of submodules.

Scarab's PackageBuilder provides the framework for using multiply-nested submodules.  To test these features, we need to be able to make Scarab a submodule of the test project.  Therefore we've included the script `copy_scarab.sh`, which is called by CMake to copy all of the contents of the scarab directory to a subdirectory called `scarab` (excluding anything listed in the `.gitignore` file).  Then you can use the `CMakeLists.txt` file in this directory to test the submodule framework in PackageBuilder.

## Usage

```
> cd /top/level/of/Scarab/testing/package_builder/PBTest
> mkdir build
> cd build
> cmake ..
> make install
```

For development, if working on Scarab and building in a subdirectory of `testing/package_builder/PBTest`, be sure to clean scarab before re-testing the CMake process:

```
rm -rf ../scarab
cmake ..
```

## Cleanup

```
> cd /top/level/of/Scarab/testing/package_builder/PBTest
> rm -rf scarab
```
