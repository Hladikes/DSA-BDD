//
//
//
// edited form: https://gist.github.com/JHvolka/f17eab6edd8dda1f7a399fbbdd71b8d1
//
//
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>

#ifndef JHVOLKA_DNF_GENERATOR
#define JHVOLKA_DNF_GENERATOR

namespace jhvolka {

  string generateExpression(string vars) {
    std::vector<std::string> permutations; //! Terms

    // Generate all permutations of terms
    for (uint32_t i = 0; i < (1U << (vars.length())); ++i) {
      uint32_t      mask = 1u; // Mask starting at LSD
      std::stringstream tmp; // For string building

      // Iterate over literals
      for (char var: vars) {
        // Append literal. Negation depends on bit in i and mask
        tmp << ((i & mask) ? "!" : "") << var;
        mask <<= 1; // Move mast to next digit
      }
      // Append generated term to list of terms
      permutations.push_back(tmp.str());
    }

    bool first = true;

    // Initialize random device and distribution
    std::random_device        rd;
    std::mt19937          gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    std::stringstream out;
        
    // Randomly print about half ot the generated literals
    for (const auto &s: permutations) {
      if (distrib(gen)) {
        out << (first ? "" : "+") << s;
        first = false;
      }
    }

    return out.str();
  }

}

#endif