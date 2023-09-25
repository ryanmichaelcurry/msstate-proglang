#!/bin/bash
make clean
make
./tips_lex abstractions_bad.pas | diff - abstractions_bad.correct
./tips_lex abstractions_good.pas | diff - abstractions_good.correct
./tips_lex input1.pas | diff - input1.correct
./tips_lex input2.pas | diff - input2.correct
./tips_lex keywords_bad.pas | diff - keywords_bad.correct
./tips_lex keywords_good.pas | diff - keywords_good.correct
./tips_lex operators_bad.pas | diff - operators_bad.correct
./tips_lex operators_good.pas | diff - operators_good.correct
./tips_lex punctuation.pas | diff - punctuation.correct
./tips_lex unknown1.pas | diff - unknown1.correct
./tips_lex unknown2.pas | diff - unknown2.correct
