#ifndef _midge_macros_hh_
#define _midge_macros_hh_

#define accessible( x_type, x_variable )\
    public:\
        void set_##x_variable( const x_type& p_variable )\
        {\
            f_##x_variable = p_variable;\
            return;\
        }\
        const x_type& get_##x_variable() const\
        {\
            return f_##x_variable;\
        }\
    protected:\
        x_type f_##x_variable;

#define referrable( x_type, x_variable )\
    public:\
        const x_type& x_variable() const\
        {\
            return f_##x_variable;\
        }\
        x_type& x_variable()\
        {\
            return f_##x_variable;\
        }\
    protected:\
        x_type f_##x_variable;

#define assignable( x_type, x_variable )\
    public:\
        void set_##x_variable( x_type* p_variable )\
        {\
            f_##x_variable = p_variable;\
            return;\
        }\
        x_type* get_##x_variable() const\
        {\
            return f_##x_variable;\
        }\
    protected:\
        x_type* f_##x_variable;


#endif
