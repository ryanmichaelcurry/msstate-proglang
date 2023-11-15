#!/bin/bash
make clean
make
echo '4'
./parse.exe -p -d 4-if_convert.pas | diff - 4-if_convert.correct
./parse.exe -p -d 4-if_convert.pas > 4-if_convert.test