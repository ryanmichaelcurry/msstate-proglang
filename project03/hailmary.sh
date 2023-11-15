#!/bin/bash
make clean
make
echo '2'
./parse.exe -p -d 2-areas.pas | diff - 2-areas.correct
./parse.exe -p -d 2-areas.pas > 2-areas.test