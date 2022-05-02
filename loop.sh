#!/bin/bash
for ((i = 13; i <= 26; i++))
do
  echo -e "\nTesting for " $i
  ./test $i 10 >>$i.10.txt &
done
