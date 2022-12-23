#!/usr/bin
make
insmod assignment1.ko time=10
dmesg |tail -15