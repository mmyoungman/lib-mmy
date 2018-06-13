#!/bin/bash
flags="-ggdb"

gcc $flags -D DEBUG tests.c -o tests-debug
./tests-debug
