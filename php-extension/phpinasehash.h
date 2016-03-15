#ifndef PHP_INASEHASH_H

#define PHP_INASEHASH_H 1

#define PHP_INASEHASH_VERSION "1.1"
#define PHP_INASEHASH_EXTNAME "inasehash"

PHP_FUNCTION(inasehash);
PHP_FUNCTION(inaseunhash);

extern zend_module_entry inasehash_module_entry;
#define phpext_inasehash_ptr &inasehash_module_entry

#endif
