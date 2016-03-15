/*
 * Inasehash Ruby implementation using the foreign language interface.
 *
 * Created by Markus Wanke on 
 */
#include <stdint.h>

#include "ruby.h"
#include "inasehash.h"

#define RAISE_AND_RETURN_IF(expr, extype, msg) if(expr) { rb_raise(extype, msg); return Qnil; }


// The module itself 
VALUE Inasehash = Qnil;

// Prototypes 
VALUE method_hash(VALUE, VALUE);
VALUE method_unhash(VALUE, VALUE);
VALUE method_isvalid(VALUE, VALUE);

// Modole initialization
void Init_inasehash() 
{
    Inasehash = rb_define_module("Inasehash");
    rb_define_module_function(Inasehash, "hash", method_hash, 1);
    rb_define_module_function(Inasehash, "unhash", method_unhash, 1);
    rb_define_module_function(Inasehash, "isvalid?", method_isvalid, 1);
}


VALUE method_hash(VALUE self, VALUE hint_rb)
{
    RAISE_AND_RETURN_IF( TYPE(hint_rb) != T_FIXNUM && TYPE(hint_rb) != T_BIGNUM, rb_eTypeError, 
            "invalid parameter type, number (fix or big-num) expected");

    uint64_t hval = NUM2ULL(hint_rb);
    char *hash = inasehash_static(hval);
    return rb_str_new2(hash);
}

VALUE method_unhash(VALUE self, VALUE hstr_rb)
{
    RAISE_AND_RETURN_IF( TYPE(hstr_rb) != T_STRING, rb_eTypeError, 
            "invalid parameter type, string expected");

    char *hash = StringValueCStr(hstr_rb);

    RAISE_AND_RETURN_IF( !hash, rb_eTypeError, "ruby-string not convertiable to c-string");


    RAISE_AND_RETURN_IF( !is_valid_hash_string(hash), rb_eArgError, 
            "inaseunhash internal exception. hashstring hash wrong format. should be [a-zA-Z0-9_-]{11}");

    uint64_t hval = inaseunhash(hash);
    return ULL2NUM(hval);
}

VALUE method_isvalid(VALUE self, VALUE hstr_rb)
{
    RAISE_AND_RETURN_IF( TYPE(hstr_rb) != T_STRING, rb_eTypeError, 
            "invalid parameter type, string expected");

    char *hash = StringValueCStr(hstr_rb);

    RAISE_AND_RETURN_IF( !hash, rb_eTypeError, "ruby-string not convertiable to c-string");

    return is_valid_hash_string(hash) ? Qtrue : Qfalse;
}
