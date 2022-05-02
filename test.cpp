#include <iostream>
#include <bitset>
#include <chrono>
#include "bdd.hpp"

#define MEASURE(tag, code) \
  { \
    cout << tag; \
    auto start = chrono::steady_clock::now(); \
    code; \
    auto end = chrono::steady_clock::now(); \
    cout << " took " \
              << (double) (chrono::duration_cast<chrono::nanoseconds> \
                      (end - start).count()) / 1000000000.0 << "s" \
              << endl; \
  }

// Formula - !A!L!YD+!UZ!M!SA+!EYCGK+P!NIXF+ZIL!KX
// poradie - ACDEFGIKLMNPSUWXYZ 
int main(int argc, char** argv) {
  string expression = "AB+CD+EF+GH";
  string order = "ABCDEFGH";

  BDD bdd;
  MEASURE("create", {
    bdd.create(expression, order);
  });

  // Generating all possible combinations of input
  size_t orderLength = order.length();
  size_t rowsCount = EXP2(orderLength);
  
  vector<string> combinations;
  string out = "";

  for (size_t n = 0; n < rowsCount; n++) {
    string bin = bitset<128>(n).to_string();
    combinations.push_back(bin.substr(128 - orderLength));
  }
  
  MEASURE("integrity check; use", {
    for (auto variant : combinations) {
      out.push_back(bdd.use(variant));
    }
  });

  cout << ((out == bdd.vector) ? "vectors are equal" : "vectors are not equal") << endl;

  return 0;
}