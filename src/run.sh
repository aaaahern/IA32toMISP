#!/bin/bash

make

for input in $(ls ../tst/); do
    echo ./IA32toMISP ../tst/$input ../out/$input
    ./IA32toMISP ../tst/$input ../out/$input
done
