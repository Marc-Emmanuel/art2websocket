#!/bin/bash
# Compile the project and run it
# Make sure boost headers are in the path
export DYLD_LIBRARY_PATH=.:/opt/local/lib/
# make clean
# make
# ./ART2WebSocket localhost 5000 5000 $1
./ART2WebSocket pixls 5000 5000 $1