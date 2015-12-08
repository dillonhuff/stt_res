#include "src/ast.h"
#include "src/context.h"
#include "test/context_tests.h"
#include "test/test_utils.h"

namespace stt_res {

  void make_var() {
    context c;
    auto a1 = c.mk_var("a");
    auto a2 = c.mk_var("a");
    test_assertion(*a1 == *a2, "make_var");
  }

  void sub_var() {
    
  }

  void all_context_tests() {
    make_var();
    sub_var();
  }

}
