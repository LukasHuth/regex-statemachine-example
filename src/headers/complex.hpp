#pragma once
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <iostream>
#include <sstream>
namespace complex {
bool matches(std::string,std::string);
class StateMachinePart {
private:
  char matching_char;
  bool is_any;
  bool is_start;
  bool has_direct_path_to_end;
  bool is_optional = false;
  std::unordered_map<char, StateMachinePart*> next;
  bool hasNextKey(char c);
  bool hasAnyKey();
  bool matches(char c);
  bool test(std::string);
  StateMachinePart(char c, bool is_any, bool repeating, bool has_direct_path_to_end);
  std::string toString(int indentation);
public:
  bool hasDirectPathToEnd();
  bool testStart(std::string);
  bool isOptional() { return this->is_optional; }
  void setDirectPathToEnd(bool value) { this->has_direct_path_to_end = value; }
  void skipLink(StateMachinePart* other);
  void link(char c, StateMachinePart*);
  std::string toString();
  StateMachinePart* createPathTo(char c, bool is_any, bool repeating, bool has_direct_path_to_end);
  StateMachinePart();
};
StateMachinePart generateStateMachine(std::string value);
bool matches(std::string,std::string, StateMachinePart);
}
