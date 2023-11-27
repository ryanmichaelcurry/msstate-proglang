#!/bin/bash
make clean
make
./parse.exe -p -t -s -d 1-hello.pas
./parse.exe -p -t -s -d 2-areas.pas
./parse.exe -p -t -s -d 3-bad_prog.pas
./parse.exe -p -t -s -d 4-if_convert.pas
./parse.exe -p -t -s -d 5-multiples.pas
./parse.exe -p -t -s -d 6-lotsovar.pas
./parse.exe -p -t -s -d 7-bmi_calc.pas
./parse.exe -p -t -s -d 8-mult_table.pas
./parse.exe -p -t -s -d 9-input_test.pas
./parse.exe -p -t -s -d 10-threedim.pas
./parse.exe -p -t -s -d 11-divisors.pas
./parse.exe -p -t -s -d 12-not_sample.pas