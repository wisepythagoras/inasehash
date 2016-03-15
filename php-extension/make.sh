#!/bin/bash

if [[ "$(basename $(pwd))" != "php-extension" ]]
then
    echo "ERROR! This script has to be run from within the php-extension directory"
    exit 1
fi

rm -rf build inasehash.so

mkdir build
cp phpinasehash.h phpinasehash.c config.m4 build

cd build 
phpize && ./configure && make && cp modules/inasehash.so ..

