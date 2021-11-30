#!/bin/bash
make || exit
cd ../test || exit

# testfile="test"
# testfile="dowhile"
# testfile="pureif"
# testfile=var
# testfile=arr
# testfile=fib
# testfile=cont
testfile=logic

if [ "$1" = "g" ]; then
    gdb -ex=r --args ../src/com "${testfile}.x0"
else
    ../src/com "${testfile}.x0"
fi
