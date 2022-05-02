#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <memory>

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
  size_t expressionLength = expression.length();
  size_t variablesCount = order.length();
  size_t rowsCount = EXP2(variablesCount);
  size_t tableSize = rowsCount * variablesCount;

  bool variablesLookupTable[128];
  string outputVector = "";

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
    outputVector += vectorBit ? "1" : "0";
  }

  return outputVector;
}



map<string, shared_ptr<Node>> getUniqueVectorNodeMap(const string& vector) {
  map<string, shared_ptr<Node>> vectorNodeMap;
  size_t vectorLength = vector.length();

  for (size_t n = 0; EXP2(n) <= vectorLength; n++) {
    string buffer = "";

    for (size_t i = 0; i < vectorLength; i++) {
      buffer += vector[i];

      if ((i + 1) % (vectorLength / EXP2(n)) != 0) {
        continue;
      }

      const string bufferLeft = buffer.substr(0, buffer.length() / 2);
      const string bufferRight = buffer.substr(buffer.length() / 2);

      if (bufferLeft == bufferRight) {
        buffer = "";
        continue;
      }

      vectorNodeMap.try_emplace(buffer, make_shared<Node>(buffer, n));
      buffer = "";
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
  shared_ptr<Node> parent = getClosestUniqueNode(vector, vectorNodesMap);

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

    void create(const string& expression, const string& order) {
      this->vector = calculateVector(expression, order);
      auto vectorNodesMap = getUniqueVectorNodeMap(this->vector);
      this->root = constructDiagram(this->vector, vectorNodesMap);
    };

    char use(const string& order) {
      return useDiagram(this->root, order);
    };
};

#endif