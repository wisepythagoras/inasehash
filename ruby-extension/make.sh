#!/bin/bash



if [[ -e Makefile ]]
then
    make clean
    rm Makefile
fi

ruby extconf.rb
make
