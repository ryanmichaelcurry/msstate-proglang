#!/bin/bash
make clean
make
./lab3 | diff - output.correct
./lab3 > output.test
