#!/bin/bash
flags="-ggdb -msse2 -fpermissive -Wno-write-strings"

g++ $flags -D DEBUG tests.cpp -o tests-debug
