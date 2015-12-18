#include "src/ast.h"
#include "src/context.h"
#include "test/catch.hpp"

namespace stt_res {

  TEST_CASE("same_var_eq") {
    context c;
    auto k = c.mk_tvar("k");
    auto a = c.mk_var("a", k);
    REQUIRE(*a == *a);
  }

  TEST_CASE("same_var_name_eq") {
    context c;
    auto k = c.mk_tvar("k");
    auto a1 = c.mk_var("a", k);
    auto a2 = c.mk_var("a", k);
    REQUIRE(*a1 == *a2);
  }

  TEST_CASE("diff_var_neq") {
    context c;
    auto k = c.mk_tvar("k");
    auto a = c.mk_var("a", k);
    auto b = c.mk_var("b", k);
    REQUIRE(*a != *b);
  }

  TEST_CASE("same_var_name_not_neq") {
    context c;
    auto k = c.mk_tvar("k");
    auto a1 = c.mk_var("a", k);
    auto a2 = c.mk_var("a", k);
    REQUIRE(!(*a1 != *a2));
  }

  TEST_CASE("same_ap_eq") {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto a = c.mk_var("a", k);
    auto fa = c.mk_ap(f, a);
    REQUIRE(*fa == *fa);
  }

  TEST_CASE("diff_ap_neq") {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto a = c.mk_var("a", k);
    auto b = c.mk_var("b", k);
    auto fa = c.mk_ap(f, a);
    REQUIRE(*fa != *b);
  }

  TEST_CASE("diff_lam_neq") {
    context c;
    auto k = c.mk_tvar("k");
    auto x = c.mk_var("x", k);
    auto y = c.mk_var("y", k);
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto fx = c.mk_ap(f, x);
    auto lfx = c.mk_lam(x, fx);
    auto ly = c.mk_lam(x, y);
    REQUIRE(*lfx != *ly);
  }

  TEST_CASE("same_lam_eq") {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, k);
    auto x = c.mk_var("x", k);
    auto f = c.mk_var("f", ft);
    auto fx = c.mk_ap(f, x);
    auto lfx1 = c.mk_lam(x, fx);
    auto lfx2 = c.mk_lam(x, fx);
    REQUIRE(*lfx1 == *lfx2);
  }

  TEST_CASE("lam_free_vars") {
    context c;
    auto k = c.mk_tvar("k");
    auto ft = c.mk_tfunc(k, c.mk_tfunc(k, k));
    auto x1 = c.mk_var("x", k);
    auto x2 = c.mk_var("x", k);
    auto y = c.mk_var("y", k);
    auto f1 = c.mk_var("f", ft);
    auto f2 = c.mk_var("f", ft);
    auto fx = c.mk_ap(f1, x1);
    auto fxy = c.mk_ap(fx, y);
    auto lfxy = c.mk_lam(f2, fxy);
    auto lxlfxy = c.mk_lam(x2, lfxy);
    auto fvs = free_vars(lxlfxy);
    REQUIRE(fvs.size() == 1);
    REQUIRE(*(fvs[0]) == *y);
  }
  
}
