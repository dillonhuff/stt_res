#include "src/ast.h"
#include "test/term_tests.h"
#include "test/test_utils.h"

namespace stt_res {

  void same_var_eq() {
    tvar k("k");
    var a("a", &k);
    test_assertion(a == a, "same_var_eq");
  }

  void same_var_name_eq() {
    tvar k("k");
    var a1("a", &k);
    var a2("a", &k);
    test_assertion(a1 == a2, "same_var_name_eq");
  }

  void diff_var_neq() {
    tvar k("k");
    var a("a", &k);
    var b("b", &k);
    test_assertion(a != b, "diff_var_neq");
  }

  void same_var_name_not_neq() {
    tvar k("k");
    var a1("a", &k);
    var a2("a", &k);
    test_assertion(!(a1 != a2), "same_var_name_not_neq");
  }

  void same_ap_eq() {
    tvar k("k");
    tfunc ft(&k, &k);
    var f("f", &ft);
    var a("a", &k);
    ap fa(&f, &a);
    test_assertion(fa == fa, "same_ap_eq");
  }

  void diff_ap_neq() {
    tvar k("k");
    tfunc ft(&k, &k);
    var f("f", &ft);
    var a("a", &k);
    var b("b", &k);
    ap fa(&f, &a);
    test_assertion(fa != b, "diff_ap_neq");
  }

  void diff_lam_neq() {
    tvar k("k");
    var x("x", &k);
    var y("y", &k);
    tfunc ft(&k, &k);
    var f("f", &ft);
    ap fx(&f, &x);
    lam lfx(&x, &fx);
    lam ly(&x, &y);
    test_assertion(lfx != ly, "diff_lam_neq");
  }

  void same_lam_eq() {
    tvar k("k");
    tfunc ft(&k, &k);
    var x("x", &k);
    var f("f", &ft);
    ap fx(&f, &x);
    lam lfx1(&x, &fx);
    lam lfx2(&x, &fx);
    test_assertion(lfx1 == lfx2, "same_lam_eq");
  }

  void lam_free_vars() {
    tvar k("k");
    tfunc ft(&k, &k);
    var x1("x", &k);
    var x2("x", &k);
    var y("y", &k);
    var f1("f", &ft);
    var f2("f", &ft);
    ap xy(&x1, &y);
    ap fxy(&f1, &xy);
    lam lfxy(&f2, &fxy);
    lam lxlfxy(&x2, &lfxy);
    auto fvs = free_vars(&lxlfxy);
    test_assertion(fvs.size() == 1 && *(fvs[0]) == y, "lam_free_vars");
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
