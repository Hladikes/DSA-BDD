#!/bin/bash
find ./measurement/ -name '*.json' -delete

echo -e min $1 
echo -e max $2 
echo -e fncount $3 

for ((i = $1; i <= $2; i++))
do
  echo -e "Testing for" $i
  ./test $i $3 >>./measurement/${i}.${3}.json &
done
wait