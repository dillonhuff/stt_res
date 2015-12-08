#include <iomanip>
#include <iostream>

#include "test/test_utils.h"

void test_utils(bool assertion, std::string test_name) {
  std::cout << std::setw(50) << std::left << test_name;
  std::cout << " ";
  if (assertion) {
    std::cout << std::setw(10) << std::left << " passed" << std::endl;
  } else {
    std::cout << std::setw(10) << test_name << " FAILED" << std::endl;
  }
  
}
