#!/bin/sh
make lex || exit
cat ../test/comment.lex | ./lex