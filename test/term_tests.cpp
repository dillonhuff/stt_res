#include "src/ast.h"
#include "test/term_tests.h"
#include "test/test_utils.h"

namespace stt_res {

  void same_var_eq() {
    var a("a");
    test_assertion(a == a, "same_var_eq");
  }

  void same_var_name_eq() {
    var a1("a");
    var a2("a");
    test_assertion(a1 == a2, "same_var_name_eq");
  }

  void diff_var_neq() {
    var a("a");
    var b("b");
    test_assertion(a != b, "diff_var_neq");
  }

  void same_var_name_not_neq() {
    var a1("a");
    var a2("a");
    test_assertion(!(a1 != a2), "same_var_name_not_neq");
  }

  void same_ap_eq() {
    var f("f");
    var a("a");
    ap fa(&f, &a);
    test_assertion(fa == fa, "same_ap_eq");
  }

  void diff_ap_neq() {
    var f("f");
    var a("a");
    var b("b");
    ap fa(&f, &a);
    test_assertion(fa != b, "diff_ap_neq");
  }

  void all_term_tests() {
    same_var_eq();
    same_var_name_eq();
    diff_var_neq();
    same_var_name_not_neq();
    same_ap_eq();
    diff_ap_neq();
  }
  
}
