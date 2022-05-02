#!/bin/bash
for ((i = 13; i <= 20; i++))
do
  echo -e "\nTesting for " $i
  echo -e "\n"
  ./test $i 10
done
