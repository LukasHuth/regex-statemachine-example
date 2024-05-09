#include "headers/main.hpp"

#define is_active(x) enabled_tests.find(x) != enabled_tests.end()

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
const int test_amount = 7;
int main(int argc, char* argv[]) {
  std::unordered_set<int> enabled_tests;
  if(argc == 1) {
    for(int i = 1; i <= test_amount; i++) {
      enabled_tests.insert(i);
    }
  } else {
    for(int i = 1; i < argc; i++) {
      int result = atoi(argv[i]);
      enabled_tests.insert(result);
    }
  }
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
  // To test it against more functions, just add the pointer to the function to this vector
  std::vector<bool(*)(std::string,std::string,complex::StateMachinePart)> funcs;
  funcs.push_back(simple::matches_t);
  funcs.push_back(complex::matches);
  // To add more tests just add the regex, value, expected result and generated statemachine for the regex to this function
  std::vector<data_holder> args;
  if(is_active(1))args.push_back(data_holder{test_regex1, test_value1, true, complex::generateStateMachine(test_regex1)});
  if(is_active(2))args.push_back(data_holder{test_regex2, test_value2, true, complex::generateStateMachine(test_regex2)});
  if(is_active(3))args.push_back(data_holder{test_regex3, test_value3, true, complex::generateStateMachine(test_regex3)});
  if(is_active(4))args.push_back(data_holder{test_regex4, test_value4, true, complex::generateStateMachine(test_regex4)});
  if(is_active(5))args.push_back(data_holder{test_regex5, test_value5, false, complex::generateStateMachine(test_regex5)});
  if(is_active(6))args.push_back(data_holder{test_regex6, test_value6, true, complex::generateStateMachine(test_regex6)});
  if(is_active(7))args.push_back(data_holder{test_regex7, test_value7, true, complex::generateStateMachine(test_regex7)});
  test(funcs, args);
  measure(funcs, args);
}

const std::string function_names[2] = {"simple::match", "complex::match"};
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
    std::cout << "the " << function_names[func_count++] << " function took " << micro_sum.count() / ((double) amount) << "μs per iteration for 1 million iterations" << std::endl;
  }
}
void test(
  std::vector<bool(*)(std::string,std::string, complex::StateMachinePart)> funcs,
  std::vector<data_holder> args
)
{
  for(auto function : funcs) {
    // int position = 0;
    for(struct data_holder dh : args) {
      // std::cout << "arg position: " << position++ << std::endl;
      assert(dh.expected == function(dh.regex, dh.value, dh.state_machine));
    }
  }
}
