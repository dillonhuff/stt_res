#include <iomanip>
#include <iostream>

#include "test/test_utils.h"

namespace stt_res {
  
  void test_assertion(bool assertion, std::string test_name) {
    std::cout << std::setw(50) << std::left << test_name;
    std::cout << " ";
    if (assertion) {
      std::cout << std::setw(10) << std::left << " passed" << std::endl;
    } else {
      std::cout << std::setw(10) << " FAILED" << std::endl;
    }
  
  }

}
