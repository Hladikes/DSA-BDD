#!/bin/bash
for ((i = 13; i <= 26; i++))
do
  echo -e "\nTesting for " $i
  ./test $i 10 >>./measurement/$i.10.txt &
done
