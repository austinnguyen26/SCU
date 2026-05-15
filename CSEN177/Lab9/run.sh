#!/bin/bash
cat /dev/urandom | head -c 100000 > file1.bin
cat /dev/urandom | head -c 1000000 > file2.bin
cat /dev/urandom | head -c 10000000 > file3.bin
cat /dev/urandom | head -c 100000000 > file4.bin