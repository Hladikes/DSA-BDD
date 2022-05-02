#include <iostream>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <sstream>

#include "bdd.hpp"

#define RANDBOOL (rand() % 2 == 1)

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

#define MEASURE_SAVE(durationvar, code) \
  { \
    auto start = chrono::steady_clock::now(); \
    code; \
    auto end = chrono::steady_clock::now(); \
    durationvar = (double) (chrono::duration_cast<chrono::nanoseconds>(end - start).count()) / 1000000000.0; \
  }

string generateOrder(size_t length) {
  string order = "";
  for (size_t idx = 0; idx < length; idx++) {
    order.push_back(idx + 65);
  }
  return order;
}

string generateRandomExpression(string order) {
  string output = "";  

  for (size_t idx = 0; idx < order.length() - 1; idx++) {
    if (idx != 0 && RANDBOOL) output += "+";
    if (RANDBOOL) output += "!";
    output += order[idx];
  }

  return output;
}


template<class T>
string printJSON (string key, T value) {
  stringstream ss;
  ss << "\"" << key << "\": ";
  
  if constexpr (is_arithmetic<T>::value) {
    ss << value;
  }
  
  else  {
    ss << "\"" << value << "\"";
  }
  
  return ss.str();
}

int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Invalid arguments; ./test <number of variables> <number of repeat cycles>" << endl;
    return 1;
  }

  size_t orderLength = atoi(argv[1]);
  size_t numberOfCycles = atoi(argv[2]);

  string order = generateOrder(orderLength);
  
  // cout << "expression,createDuration,useDuraion,vectorsEqual" << endl;
  cout << "[" << endl;

  for (size_t count = 0; count < numberOfCycles; count++) {
    string expression = generateRandomExpression(order);
    
    double createDuration = 0;
    double useDuration = 0;

    BDD bdd;

    MEASURE_SAVE(createDuration, {
      bdd.create(expression, order);
    });

    // Generating all possible combinations of input
    size_t orderLength = order.length();
    size_t rowsCount = EXP2(orderLength);
    
    vector<string> combinations;
    string out = "";

    for (size_t n = 0; n < rowsCount; n++) {
      string bin = bitset<1024>(n).to_string();
      combinations.push_back(bin.substr(1024 - orderLength));
    }

    MEASURE_SAVE(useDuration, {
      for (const auto& variant : combinations) {
        out.push_back(bdd.use(variant));
      }
    });

    cout 
      << "\t{" << endl
      << "\t\t" << printJSON<string>("expression", expression) << "," << endl
      << "\t\t" << printJSON<double>("createDuration", createDuration) << "," << endl
      << "\t\t" << printJSON<double>("useDuration", useDuration) << "," << endl
      << "\t\t" << printJSON<double>("nodesCount", bdd.nodesCount) << "," << endl
      << "\t\t" << printJSON<double>("reducedNodesCount", bdd.reducedNodesCount) << "," << endl
      << "\t\t" << printJSON<bool>("vectorsAreEqual", out == bdd.vector) << endl
      << "\t}" 
      << ((count != numberOfCycles - 1) ? "," : " ")
      << endl;
  }

  cout << "]" << endl;

  return 0;
}