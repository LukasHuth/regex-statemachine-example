#pragma once
#include "complex.hpp"
#include <string>
#include <iostream>
namespace simple {
bool matches(std::string,std::string);
inline bool matches_t(std::string a,std::string b, complex::StateMachinePart) {
  return matches(a, b);
}
}
