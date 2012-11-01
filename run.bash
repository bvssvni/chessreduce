#!/bin/bash
# Compile and run.
cd chess-reduce
gcc -shared -o chess-reduce.so chess-reduce.c
if [ "$?" = "0" ];
then
	mv chess-reduce.so ../chess-reduce.so
	cd ..
	gcc -o test-chess-reduce *.so test-chess-reduce.c
	if [ "$?" = "0" ];
	then
		time ./test-chess-reduce
	fi
fi