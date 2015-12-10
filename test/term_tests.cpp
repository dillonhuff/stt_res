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
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto fx = c.mk_ap(f, x);
    auto lfx = c.mk_lam(x, fx);
    auto ly = c.mk_lam(x, y);
    test_assertion(*lfx != *ly, "diff_lam_neq");
  }

  void same_lam_eq() {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto x = c.mk_var("x", k);
    auto f = c.mk_var("f", ft);
    auto fx = c.mk_ap(f, x);
    auto lfx1 = c.mk_lam(x, fx);
    auto lfx2 = c.mk_lam(x, fx);
    test_assertion(*lfx1 == *lfx2, "same_lam_eq");
  }

  void lam_free_vars() {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto x1 = c.mk_var("x", k);
    auto x2 = c.mk_var("x", k);
    auto y = c.mk_var("y", k);
    auto f1 = c.mk_var("f", ft);
    auto f2 = c.mk_var("f", ft);
    auto xy = c.mk_ap(x1, y);
    auto fxy = c.mk_ap(f1, xy);
    auto lfxy = c.mk_lam(f2, fxy);
    auto lxlfxy = c.mk_lam(x2, lfxy);
    auto fvs = free_vars(lxlfxy);
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
