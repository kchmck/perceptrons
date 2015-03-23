#!/usr/bin/bash

./convert-train.py ../train.dat train
./convert-test.py ../devel.dat devel
./convert-test.py ../test.dat test
