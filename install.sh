#!/bin/bash
# Install packages

cd $(dirname $0)

make clean
make
make install
make cleanobjs
clear
adaptiveserver


