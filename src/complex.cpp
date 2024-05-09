#define DEBUG_
#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << x << std::endl;
#else
#define DEBUG_PRINT(x)
#endif
#include "headers/complex.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace complex {
const char any_char = 1;
const char start_char = 2;
StateMachinePart::StateMachinePart() {
  this->matching_char = '\0';
  this->is_any = false;
  this->is_start = true;
  this->has_direct_path_to_end = false;
}
std::string StateMachinePart::toString() {
  return this->toString(0);
}
std::string StateMachinePart::toString(int indentation) {
  std::stringstream ss;
  std::string indent(indentation, ' ');

  ss << indent << "Matching Character: " << matching_char << std::endl;
  ss << indent << "Is Any: " << (is_any ? "true" : "false") << std::endl;
  ss << indent << "Is Start: " << (is_start ? "true" : "false") << std::endl;
  ss << indent << "Has Direct Path to End: " << (has_direct_path_to_end ? "true" : "false") << std::endl;
  ss << indent << "Is Optional: " << (is_optional ? "true" : "false") << std::endl;

  ss << indent << "Next: {" << std::endl;
  for(auto& entry : next) {
    ss << indent << "  '" << entry.first << "': ";
    if(entry.second == this) {
      ss << "self" << std::endl;
    } else {
      ss << std::endl << entry.second->toString(indentation + 4);
    }
  }
  ss << indent << "}" << std::endl;
  return ss.str();
}
void StateMachinePart::link(char c, StateMachinePart* other) {
  this->next[c] = other;
}
bool StateMachinePart::matches(char c) {
  if(this->is_any) return true;
  return this->matching_char == c;
}
bool StateMachinePart::test(std::string value) {
  DEBUG_PRINT("testing: " << value)
  if(value.length() == 0) return this->has_direct_path_to_end;
  if(value.length() == 0) return this->has_direct_path_to_end;
  char next_char = value[0];
  DEBUG_PRINT("searching for: " << next_char)
  if(this->hasNextKey(next_char)) return this->next[next_char]->test(value.substr(1));
  if(this->hasAnyKey()) return this->next[any_char]->test(value.substr(1));
  return false;
}
bool StateMachinePart::hasDirectPathToEnd() {
  return this->has_direct_path_to_end;
}
bool StateMachinePart::hasAnyKey() {
  return this->hasNextKey(any_char);
}
bool StateMachinePart::hasNextKey(char c) {
  return this->next.find(c) != this->next.end();
}
bool StateMachinePart::testStart(std::string value) {
  if(!this->is_start) return this->test(value);
  if(value.length() == 0) return this->has_direct_path_to_end;
  DEBUG_PRINT("testing: " << value)
  char next_char = value[0];
  if(this->hasNextKey(next_char)) return this->next[next_char]->test(value.substr(1));
  if(this->hasAnyKey()) return this->next[any_char]->test(value.substr(1));
  DEBUG_PRINT("Beeing here")
  return false;
}
void generateStateMachineTo(std::string value, StateMachinePart* node) {
  if(value.length() == 0) {
    std::cerr << "Entered generateStateMachineTo with an empty string" << std::endl;
    exit(1);
  }
  if(value.length() == 1) {
    node->createPathTo(value[0], value[0] == '.', false, true);
    return;
  }
  DEBUG_PRINT("trying for value: " << value)
  char next_char = value[0];
  bool is_any = next_char == '.';
  long unsigned int repeating_offset = 1;
  long unsigned int replace_size = 1;
  if(next_char == '\\') {
    if(value.length() == 1) {
      std::cerr << "Tried excape sequence without adding something behind it" << std::endl;
      exit(2);
    }
    next_char = value[1];
    repeating_offset++;
    replace_size++;
  }
  bool repeating = (value.length() > repeating_offset) ? value[repeating_offset] == '*' : false;
  if(repeating) replace_size++;
  bool direct_path_to_end = value.length() == replace_size;
  DEBUG_PRINT("next_char: " << next_char << "|is_any: " << (is_any ? "true" : "false") << "|repeating:" << (repeating ? "true" : "false"))
  StateMachinePart* element = node->createPathTo(next_char, is_any, repeating, direct_path_to_end);
  if(!direct_path_to_end) {
    DEBUG_PRINT("value = " << value << std::endl << "value.substr(" << replace_size << ") = " << value.substr(replace_size))
    generateStateMachineTo(value.substr(replace_size), element);
  }
  if(element->isOptional()) {
    DEBUG_PRINT("optional: value: " << value)
    node->skipLink(element);
  }
}
StateMachinePart* StateMachinePart::createPathTo(char c, bool is_any, bool repeating, bool direct_path_to_end) {
  StateMachinePart* smp = new StateMachinePart(c, is_any, repeating, direct_path_to_end);
  this->next[smp->matching_char] = smp;
  return smp;
}
void StateMachinePart::skipLink(StateMachinePart* other) {
  for(const auto& pair : other->next) {
    if(!this->hasNextKey(pair.first)) {
      this->next[pair.first] = pair.second;
    }
  }
  if(other->has_direct_path_to_end) this->has_direct_path_to_end = true;
}
StateMachinePart generateStateMachine(std::string value) {
  StateMachinePart start = StateMachinePart();
  generateStateMachineTo(value, &start);
  return start;
}
StateMachinePart::StateMachinePart(char c, bool is_any, bool repeating, bool direct_path_to_end) {
  this->matching_char = c;
  this->is_any = is_any;
  if(is_any) this->matching_char = any_char;
  this->has_direct_path_to_end = direct_path_to_end;
  this->is_optional = repeating;
  if(repeating) {
    this->next[this->matching_char] = this;
  }
}
// const std::unordered_map<std::string, StateMachinePart> state_machines;
bool matches(std::string regex, std::string value) {
  StateMachinePart start = generateStateMachine(regex);
  DEBUG_PRINT("statemachine:" << std::endl << start.toString())
  return start.testStart(value);
}
bool matches(std::string regex, std::string value, StateMachinePart start) {
  DEBUG_PRINT("regex: " << regex << " statemachine:" << std::endl << start.toString())
  return start.testStart(value);
}
}
