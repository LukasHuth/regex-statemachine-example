#include "headers/simple.hpp"
#include <iostream>

#define DEBUG_
#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << x << std::endl
#else
#define DEBUG_PRINT(x)
#endif
namespace simple {
bool is_matching(std::string str, char c) {
  if(str.length() == 0) return false;
  if(c == '.') return true;
  return str[0] == c;
}
bool matches(std::string regex, std::string value) {
  if(regex.length() == 0) return value.length() == 0;
  char matching_character = regex[0];
  bool multiple = (regex.length() > 1) ? regex[1] == '*' : false;
  DEBUG_PRINT("matches call" << std::endl
    << "regex: " << regex << std::endl
    << "value: " << value << std::endl
    << "multiple: " << ((multiple) ? "true" : "false") << std::endl
    << "--------------");
  if(multiple) {
    int count = 0;
    while(is_matching(value.substr(count), matching_character)) {
      count++;
    }
    for(; count > 0; count--) {
      bool result = matches(regex.substr(2), value.substr(count));
      DEBUG_PRINT("result:" << ((result) ? "true" : "false") << std::endl
        << "value: " << value.substr(count) << std::endl
        << "regex: " << regex.substr(2));
      if(result) return true;
    }
    return matches(regex.substr(2), value);
  }
  if(!is_matching(value, matching_character)) return false;
  return matches(regex.substr(1), value.substr(1));
}
}
