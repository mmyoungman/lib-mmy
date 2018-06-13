#!/bin/bash
unset flags
flags="-ggdb"

gcc $flags -D DEBUG tests.c -o tests-debug
