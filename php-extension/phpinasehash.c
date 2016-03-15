#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "phpinasehash.h"
#include "../../inasehash.h"

// list of custom PHP functions provided by this extension
// set {NULL, NULL, NULL} as the last record to mark the end of list
static zend_function_entry inasehash_functions[] = {
    PHP_FE(inasehash, NULL)
    PHP_FE(inaseunhash, NULL)
    {NULL, NULL, NULL}
};

// the following code creates an entry for the module and registers it with Zend.
zend_module_entry inasehash_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_INASEHASH_EXTNAME,
    inasehash_functions,
    NULL, // name of the MINIT function or NULL if not applicable
    NULL, // name of the MSHUTDOWN function or NULL if not applicable
    NULL, // name of the RINIT function or NULL if not applicable
    NULL, // name of the RSHUTDOWN function or NULL if not applicable
    NULL, // name of the MINFO function or NULL if not applicable
    PHP_INASEHASH_VERSION,
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(inasehash)

PHP_FUNCTION(inasehash)
{ 
    int64_t id = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE)
        RETURN_NULL();

    char buf[12] = { 0 };
    RETURN_STRING(inasehash(id, buf), 1);
}

PHP_FUNCTION(inaseunhash)
{
    char *hash = NULL;
    size_t hash_len = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hash, &hash_len) == FAILURE)
        RETURN_NULL();

    if( hash_len != HASH_LENGTH || !is_valid_hash_string(hash))
        RETURN_NULL();

    RETURN_LONG(inaseunhash(hash));
}

// a little bit dirty but prevents a lot of linking problems regarding automake
#include "../../inasehash.c"


