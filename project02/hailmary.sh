#!/bin/bash
make clean
make
echo '1'
./tips_parse 1-hello.pas | diff - 1-hello.correct
./tips_parse 1-hello.pas > 1-hello.test
echo '2'
./tips_parse 2-areas.pas | diff - 2-areas.correct
./tips_parse 2-areas.pas > 2-areas.test
echo '3'
./tips_parse 3-bad_prog.pas | diff - 3-bad_prog.correct
./tips_parse 3-bad_prog.pas > 3-bad_prog.test
echo '4'
./tips_parse 4-if_convert.pas | diff - 4-if_convert.correct
./tips_parse 4-if_convert.pas > 4-if_convert.test
echo '5'
./tips_parse 5-multiples.pas | diff - 5-multiples.correct
./tips_parse 5-multiples.pas > 5-multiples.test
echo '6'
./tips_parse 6-lotsovar.pas | diff - 6-lotsovar.correct
./tips_parse 6-lotsovar.pas > 6-lotsovar.test
echo '7'
./tips_parse 7-bmi_calc.pas | diff - 7-bmi_calc.correct
./tips_parse 7-bmi_calc.pas > 7-bmi_calc.test
echo '8'
./tips_parse 8-mult_table.pas | diff - 8-mult_table.correct
./tips_parse 8-mult_table.pas > 8-mult_table.test