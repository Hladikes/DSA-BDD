#!/bin/bash
find ./measurement/ -name '*.json' -delete

for ((i = 13; i <= 16; i++))
do
  echo -e "Testing for" $i
  ./test $i 2 >>./measurement/${i}.10.json &
done
