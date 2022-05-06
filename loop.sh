#!/bin/bash

# $1 min number of variables
# $2 max number of variables
# $3 number of functions per job
# $4 number of paralel jobs

for ((i = $1; i <= $2; i++))
do
  mkdir ./measurement/${i}_vars

  echo -e "Testing for" $i
  
  for ((j = 1; j <= $4; j++)) {
    ./bin/test $i $3 >>./measurement/${i}_vars/${i}v.${3}fn.${j}.json &
  }

  wait
done

zip -r measurement/measurement_${1}_${2}_${3}.zip measurement