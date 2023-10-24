#!/bin/bash
make clean
make

F=input1; cat $F.in; ./parse $F.in | diff - $F.correct
F=input2; cat $F.in; ./parse $F.in | diff - $F.correct
F=input3; cat $F.in; ./parse $F.in | diff - $F.correct
F=input4; cat $F.in; ./parse $F.in | diff - $F.correct
F=input5; cat $F.in; ./parse $F.in | diff - $F.correct
F=input6; cat $F.in; ./parse $F.in | diff - $F.correct
