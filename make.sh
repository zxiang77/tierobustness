#!/bin/bash
pwd
rm ../src/global.h.gch
rm ../src/a.out
gcc ../src/* -I/usr/local/include/igraph -L/usr/local/lib -ligraph
