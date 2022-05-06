#include <iostream>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <random>
#include <set>

#include "bdd.hpp"

#define MEASURE_SAVE(durationvar, code) \
  { \
    auto start = chrono::steady_clock::now(); \
    code; \
    auto end = chrono::steady_clock::now(); \
    durationvar = (double) (chrono::duration_cast<chrono::nanoseconds>(end - start).count()) / 1000000000.0; \
  }

// https://martin.ankerl.com/2018/12/08/fast-random-bool/
bool generatRandomBool() {
  mt19937 rng(random_device{}());
  return uniform_int_distribution<>{0, 1}(rng);
}

string generateOrder(size_t length) {
  string order = "";
  for (size_t idx = 0; idx < length; idx++) {
    order.push_back(idx + 65);
  }
  return order;
}

string generateRandomExpression(string order) {
  const size_t numberOfClauses = EXP2(order.length()) / 10;

  string output = "";
  set<string> clauses;

  while (clauses.size() != numberOfClauses) {
    string clause = "";
    
    for (size_t idx = 0; idx < order.length(); idx++) {
      if (generatRandomBool()) clause += "!";
      clause += order[idx];
    }

    clauses.insert(clause);
  }

  for (const string clause : clauses) {
    output += clause + "+";
  }

  return output.substr(0, output.length() - 1);
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
      << "  {" << endl
      << "    " << printJSON<size_t>("expressionLength", expression.length()) << "," << endl
      << "    " << printJSON<double>("vectorEvaluationDuration", bdd.vectorEvaluationDuration) << "," << endl
      << "    " << printJSON<double>("diagramConstructionDuration", bdd.diagramConstructionDuration) << "," << endl
      << "    " << printJSON<double>("createDuration", createDuration) << "," << endl
      << "    " << printJSON<double>("useDuration", useDuration) << "," << endl
      << "    " << printJSON<double>("nodesCountFull", bdd.nodesCountFull) << "," << endl
      << "    " << printJSON<double>("reducedNodesCount", bdd.reducedNodesCount) << "," << endl
      << "    " << printJSON<double>("reductionRate", bdd.reductionRate) << "," << endl
      << "    " << printJSON<bool>("vectorsAreEqual", out == bdd.vector) << endl
      << "  }" 
      << ((count != numberOfCycles - 1) ? "," : " ")
      << endl;
  }

  cout << "]" << endl;

  return 0;
}