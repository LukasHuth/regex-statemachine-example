#include "headers/main.hpp"
#include "headers/complex.hpp"
#include <cassert>
#include <chrono>
#include <ratio>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

struct data_holder {
  std::string regex;
  std::string value;
  bool expected;
  complex::StateMachinePart state_machine;
};

void measure(
  std::vector<bool(*)(std::string,std::string, complex::StateMachinePart)> funcs,
  std::vector<data_holder> args
);
void test(
  std::vector<bool(*)(std::string,std::string, complex::StateMachinePart)> funcs,
  std::vector<data_holder> args
);

int main() {
  std::string test_regex1 = "abc";
  std::string test_value1 = "abc";
  std::string test_regex2 = "ab.";
  std::string test_value2 = "abc";
  std::string test_regex3 = "a*";
  std::string test_value3 = "aaa";
  std::string test_regex4 = "a*b";
  std::string test_value4 = "aaab";
  std::string test_regex5 = "b*";
  std::string test_value5 = "aaa";
  std::string test_regex6 = ".*b.*c.*d.*e.*";
  std::string test_value6 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaacaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaadaaaaaaaaaaaaaaaaaaaaaaaaacaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaeaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string test_regex7 = ".*.*a.*";
  std::string test_value7 = "abbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  std::vector<bool(*)(std::string,std::string,complex::StateMachinePart)> funcs;
  funcs.push_back(simple::matches_t);
  funcs.push_back(complex::matches);
  std::vector<data_holder> args;
  args.push_back(data_holder{test_regex1, test_value1, true, complex::generateStateMachine(test_regex1)});
  args.push_back(data_holder{test_regex2, test_value2, true, complex::generateStateMachine(test_regex2)});
  args.push_back(data_holder{test_regex3, test_value3, true, complex::generateStateMachine(test_regex3)});
  args.push_back(data_holder{test_regex4, test_value4, true, complex::generateStateMachine(test_regex4)});
  args.push_back(data_holder{test_regex5, test_value5, false, complex::generateStateMachine(test_regex5)});
  args.push_back(data_holder{test_regex6, test_value6, true, complex::generateStateMachine(test_regex6)});
  args.push_back(data_holder{test_regex7, test_value7, true, complex::generateStateMachine(test_regex7)});
  test(funcs, args);
  measure(funcs, args);
}

void measure(
  std::vector<bool(*)(std::string,std::string, complex::StateMachinePart)> funcs,
  std::vector<data_holder> args
)
{
  int func_count = 0;
  for(auto function : funcs) {
    std::chrono::duration<double, std::micro> micro_sum;
    micro_sum = std::chrono::duration<double, std::micro>::zero();
    int amount = 0;
    // for(int i = 0; i < 1'000'000; i++) {
    for(int i = 0; i < 1'000; i++) {
      auto t1 = std::chrono::high_resolution_clock::now();
      for(struct data_holder dh : args) {
        function(dh.regex, dh.value, dh.state_machine);
      }
      auto t2 = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> ms_double = t2 - t1;
      micro_sum += ms_double;
      amount++;
    }
    std::cout << "the " << func_count++ << "th function took " << micro_sum.count() / ((double) amount) << "μs per iteration for 1 million iterations" << std::endl;
  }
}
void test(
  std::vector<bool(*)(std::string,std::string, complex::StateMachinePart)> funcs,
  std::vector<data_holder> args
)
{
  for(auto function : funcs) {
    int position = 0;
    for(struct data_holder dh : args) {
      std::cout << "arg position: " << position++ << std::endl;
      assert(dh.expected == function(dh.regex, dh.value, dh.state_machine));
    }
  }
}
