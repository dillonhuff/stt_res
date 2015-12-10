#include "src/ast.h"
#include "src/context.h"
#include "test/term_tests.h"
#include "test/test_utils.h"

namespace stt_res {

  void same_var_eq() {
    context c;
    auto k = c.mk_tvar("k");
    auto a = c.mk_var("a", k);
    test_assertion(*a == *a, "same_var_eq");
  }

  void same_var_name_eq() {
    context c;
    auto k = c.mk_tvar("k");
    auto a1 = c.mk_var("a", k);
    auto a2 = c.mk_var("a", k);
    test_assertion(*a1 == *a2, "same_var_name_eq");
  }

  void diff_var_neq() {
    context c;
    auto k = c.mk_tvar("k");
    auto a = c.mk_var("a", k);
    auto b = c.mk_var("b", k);
    test_assertion(*a != *b, "diff_var_neq");
  }

  void same_var_name_not_neq() {
    context c;
    auto k = c.mk_tvar("k");
    auto a1 = c.mk_var("a", k);
    auto a2 = c.mk_var("a", k);
    test_assertion(!(*a1 != *a2), "same_var_name_not_neq");
  }

  void same_ap_eq() {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto a = c.mk_var("a", k);
    auto fa = c.mk_ap(f, a);
    test_assertion(*fa == *fa, "same_ap_eq");
  }

  void diff_ap_neq() {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto a = c.mk_var("a", k);
    auto b = c.mk_var("b", k);
    auto fa = c.mk_ap(f, a);
    test_assertion(*fa != *b, "diff_ap_neq");
  }

  void diff_lam_neq() {
    context c;
    auto k = c.mk_tvar("k");
    auto x = c.mk_var("x", k);
    auto y = c.mk_var("y", k);
    tfunc ft(k, k);
    var f("f", &ft);
    ap fx(&f, x);
    lam lfx(x, &fx);
    lam ly(x, y);
    test_assertion(lfx != ly, "diff_lam_neq");
  }

  void same_lam_eq() {
    context c;
    auto k = c.mk_tvar("k");
    tfunc ft(k, k);
    auto x = c.mk_var("x", k);
    var f("f", &ft);
    ap fx(&f, x);
    lam lfx1(x, &fx);
    lam lfx2(x, &fx);
    test_assertion(lfx1 == lfx2, "same_lam_eq");
  }

  void lam_free_vars() {
    context c;
    auto k = c.mk_tvar("k");
    tfunc ft(k, k);
    var x1("x", k);
    var x2("x", k);
    auto y = c.mk_var("y", k);
    var f1("f", &ft);
    var f2("f", &ft);
    ap xy(&x1, y);
    ap fxy(&f1, &xy);
    lam lfxy(&f2, &fxy);
    lam lxlfxy(&x2, &lfxy);
    auto fvs = free_vars(&lxlfxy);
    test_assertion(fvs.size() == 1 && *(fvs[0]) == *y, "lam_free_vars");
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
