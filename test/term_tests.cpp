#include "src/ast.h"
#include "test/term_tests.h"
#include "test/test_utils.h"

namespace stt_res {

  void same_var_eq() {
    var a("a");
    var b("b");
    test_assertion(a == b, "same_var_eq");
  }
  
  void all_term_tests() {
    same_var_eq();
  }
  
}
