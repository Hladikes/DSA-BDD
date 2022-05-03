#include <iostream>
#include <string>
#include <map>
#include <random>
#include <memory>
#include <sstream>

#define EXP2(n) (1 << n)

#ifndef BINARY_DECISION_DIAGRAM
#define BINARY_DECISION_DIAGRAM

using namespace std;

struct Node {
  static size_t counter;
  size_t id;
  uint depth;
  string vector;
  shared_ptr<Node> left;
  shared_ptr<Node> right;
  Node(const string& vector, uint depth)
    : id(counter++),
      depth(depth), 
      vector(vector),
      left(nullptr),
      right(nullptr) {}; 
  
  string toString() {
    return to_string(depth) + " { " + vector +  ", id: " + to_string(id) + "}";
  }
};

size_t Node::counter = 0;



string calculateVector(const string& expression, const string& order) {
  const size_t expressionLength = expression.length();
  const size_t variablesCount = order.length();
  const size_t rowsCount = EXP2(variablesCount);
  const size_t tableSize = rowsCount * variablesCount;

  bool variablesLookupTable[128];
  stringstream outputVectorStream;

  for (size_t n = 0; n < rowsCount; n++) {
    for (size_t v = 0; v < variablesCount; v++) {
      bool targetBit = ((n >> v) & 1);
      variablesLookupTable[(int) order[variablesCount - v - 1]] = targetBit;
    }

    bool vectorBit = false;
    bool vectorBitBuffer = true;
    bool negateNext = false;

    for (int i = 0; i < expressionLength; i++) {
      char token = expression[i];

      if (token == '!') {
        negateNext = true;
        continue;
      }

      if (token == '+') {
        vectorBit |= vectorBitBuffer;

        if (vectorBitBuffer) {
          goto nextRow; 
        }
        
        vectorBitBuffer = true;
        continue;
      }

      bool assocBit = variablesLookupTable[(int) token];
      vectorBitBuffer &= negateNext ? !assocBit : assocBit;
      negateNext = false;
    }

    vectorBit |= vectorBitBuffer;

    nextRow: {};
    outputVectorStream << vectorBit ? "1" : "0";
  }

  return outputVectorStream.str();
}



map<string, shared_ptr<Node>> getUniqueVectorNodeMap(const string& vector) {
  map<string, shared_ptr<Node>> vectorNodeMap;
  const size_t vectorLength = vector.length();

  stringstream bufferStream;

  for (size_t n = 0; EXP2(n) <= vectorLength; n++) {
    for (size_t i = 0; i < vectorLength; i++) {
      bufferStream << vector[i];

      if ((i + 1) % (vectorLength / EXP2(n)) != 0) {
        continue;
      }

      const string buffer = bufferStream.str();
      const string bufferLeft = buffer.substr(0, buffer.length() / 2);
      const string bufferRight = buffer.substr(buffer.length() / 2);

      if (bufferLeft == bufferRight) {
        bufferStream.str("");
        continue;
      }

      vectorNodeMap.try_emplace(buffer, make_shared<Node>(buffer, n));
      bufferStream.str("");
    }
  }

  return vectorNodeMap;
}



shared_ptr<Node> getClosestUniqueNode(string vector, map<string, shared_ptr<Node>>& vectorNodesMap) {
  if (vectorNodesMap.contains(vector)) {
    return vectorNodesMap[vector];
  }

  return getClosestUniqueNode(vector.substr(vector.length() / 2), vectorNodesMap);
}



shared_ptr<Node> constructDiagram(string vector, map<string, shared_ptr<Node>>& vectorNodesMap) {
  const shared_ptr<Node> parent = getClosestUniqueNode(vector, vectorNodesMap);

  if (parent->vector.length() == 1) {
    return parent;
  }

  parent->left = constructDiagram(parent->vector.substr(0, parent->vector.length() / 2), vectorNodesMap);
  parent->right = constructDiagram(parent->vector.substr(parent->vector.length() / 2), vectorNodesMap);

  return parent;
}



char useDiagram(shared_ptr<Node>& diagram, const string& order) {
  auto root = diagram;

  for (size_t idx = 0; idx < order.length(); idx++) {
    if (idx != root->depth) continue;

    root = (order[idx] == '0') ? root->left : root->right;
  }

  return root->vector[0];
}



void printDiagram(string prefix, shared_ptr<Node>& node, bool left) {
  if (node != nullptr) {
    cout << prefix << "| " << endl;
    cout << prefix << (left ? "|-- " : "\\-- ") << node->toString() << endl;
    printDiagram(prefix + (left ? "|   " : "    "), node->left, true);
    printDiagram(prefix + (left ? "|   " : "    "), node->right, false);
  }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
class BDD {
  private:
    shared_ptr<Node> root = nullptr;

  public:
    string vector = "";
    size_t nodesCountFull = 0;
    size_t reducedNodesCount = 0;
    double reductionRate = 0.0;

    void create(const string& expression, const string& order) {
      this->vector = calculateVector(expression, order);
      this->nodesCountFull = vector.length() * 2;
      auto vectorNodesMap = getUniqueVectorNodeMap(this->vector);
      this->reducedNodesCount = vectorNodesMap.size();
      this->root = constructDiagram(this->vector, vectorNodesMap);

      // Calculate reduction rate
      double fullCount = static_cast<double>(this->nodesCountFull);
      double reducedCount = static_cast<double>(this->reducedNodesCount);
      this->reductionRate = (fullCount - reducedCount) / fullCount * 100.0;
    };

    char use(const string& order) {
      return useDiagram(this->root, order);
    };
};

#endif