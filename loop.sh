#!/bin/bash
find ./measurement/ -name '*.json' -delete

for ((i = $1; i <= $2; i++))
do
  echo -e "Testing for" $i
  ./test $i $3 >>./measurement/${i}.${3}.json &
done
wait