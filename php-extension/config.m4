PHP_ARG_ENABLE(inasehash, whether to enable inasehash extension,
[ --enable-inasehash   Enable inasehash extension])

if test "$PHP_INASEHASH" = "yes"; then
  AC_DEFINE(HAVE_INASEHASH, 1, [Whether you have inasehash extension])
  PHP_NEW_EXTENSION(inasehash, phpinasehash.c, $ext_shared)
fi
