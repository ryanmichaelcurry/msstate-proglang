#!/bin/bash
make clean
make
echo '1'
./parse.exe -p -d 1-hello.pas | diff - 1-hello.correct
./parse.exe -p -d 1-hello.pas > 1-hello.test
echo '2'
./parse.exe -p -d 2-areas.pas | diff - 2-areas.correct
./parse.exe -p -d 2-areas.pas > 2-areas.test
echo '3'
./parse.exe -p -d 3-bad_prog.pas | diff - 3-bad_prog.correct
./parse.exe -p -d 3-bad_prog.pas > 3-bad_prog.test
echo '4'
./parse.exe -p -d 4-if_convert.pas | diff - 4-if_convert.correct
./parse.exe -p -d 4-if_convert.pas > 4-if_convert.test
echo '5'
./parse.exe -p -d 5-multiples.pas | diff - 5-multiples.correct
./parse.exe -p -d 5-multiples.pas > 5-multiples.test
echo '6'
./parse.exe -p -d 6-lotsovar.pas | diff - 6-lotsovar.correct
./parse.exe -p -d 6-lotsovar.pas > 6-lotsovar.test
echo '7'
./parse.exe -p -d 7-bmi_calc.pas | diff - 7-bmi_calc.correct
./parse.exe -p -d 7-bmi_calc.pas > 7-bmi_calc.test
echo '8'
./parse.exe -p -d 8-mult_table.pas | diff - 8-mult_table.correct
./parse.exe -p -d 8-mult_table.pas > 8-mult_table.test
echo '9'
./parse.exe -p -d 9-input_test.pas | diff - 9-input_test.correct
./parse.exe -p -d 9-input_test.pas > 9-input_test.test
echo '10'
./parse.exe -p -d 10-threedim.pas | diff - 10-threedim.correct
./parse.exe -p -d 10-threedim.pas > 10-threedim.test
echo '11'
./parse.exe -p -d 11-divisors.pas | diff - 11-divisors.correct
./parse.exe -p -d 11-divisors.pas > 11-divisors.test
echo '12'
./parse.exe -p -d 12-not_sample.pas | diff - 12-not_sample.correct
./parse.exe -p -d 12-not_sample.pas > 12-not_sample.test

