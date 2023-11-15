#!/bin/bash
make clean
make
echo '12'
./parse.exe -p -d 12-not_sample.pas | diff - 12-not_sample.correct
./parse.exe -p -d 12-not_sample.pas > 12-not_sample.test