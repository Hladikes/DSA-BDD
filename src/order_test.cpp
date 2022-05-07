#include <iostream>
#include <chrono>
#include <string.h>

#include "bdd.hpp"

using namespace std;

int main() {
  string expression = "AB!CD!EF!GH+A!BCDE!F!GH+A!B!CDE!FGH+AB!CDEF!GH+ABCDE!FGH+AB!CDEF!GH+A!BC!D!EF!GH+AB!C!DE!FG!H";
  string order = "ABCDEFGH";
  string reversedOrder = "HGFEDCBA";

  BDD bdd1;
  bdd1.create(expression, order);
  
  BDD bdd2;
  bdd2.create(expression, reversedOrder);

  if (bdd1.vector == bdd2.vector) {
    cout << "Vectors are equal" << endl;
  } else {
    cout << "Vectors are not equal" << endl;
  }

  return 0;
}


