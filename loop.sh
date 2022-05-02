#!/bin/bash
for ((i = 13; i <= 20; i++))
do
  echo -e "Testing for" $i
  ./test $i 10 >>./measurement/${i}.10.json &
done
