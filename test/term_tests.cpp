#include "src/ast.h"
#include "src/pretty_print.h"
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

  void diff_lam_neq() {
    var x("x");
    var y("y");
    var f("f");
    ap fx(&f, &x);
    lam lfx(&x, &fx);
    lam ly(&x, &y);
    test_assertion(lfx != ly, "diff_lam_neq");
  }

  void same_lam_eq() {
    var x("x");
    var f("f");
    ap fx(&f, &x);
    lam lfx1(&x, &fx);
    lam lfx2(&x, &fx);
    test_assertion(lfx1 == lfx2, "same_lam_eq");
  }

  void lam_free_vars() {
    var x1("x");
    var x2("x");
    var y("y");
    var f1("f");
    var f2("f");
    ap xy(&x1, &y);
    ap fxy(&f1, &xy);
    lam lfxy(&f2, &fxy);
    lam lxlfxy(&x2, &lfxy);
    auto fvs = free_vars(&lxlfxy);
    cout << xy << endl;
    cout << fxy << endl;
    cout << lxlfxy << endl;
  }

  void all_term_tests() {
    same_var_eq();
    same_var_name_eq();
    diff_var_neq();
    same_var_name_not_neq();
    same_ap_eq();
    diff_ap_neq();
    diff_lam_neq();
    same_lam_eq();
    lam_free_vars();
  }
  
}
