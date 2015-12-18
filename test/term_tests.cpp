#include "src/ast.h"
#include "src/context.h"
#include "test/catch.hpp"

namespace stt_res {

  TEST_CASE("term tests", "[term]") {
    context c;
    auto k = c.mk_tcon("k");
    auto a = c.mk_var("a", k);
    auto a1 = c.mk_var("a", k);
    auto a2 = c.mk_var("a", k);
    auto b = c.mk_var("b", k);    
    auto ft = c.mk_tfunc(k, k);
    auto f = c.mk_var("f", ft);
    auto x = c.mk_var("x", k);
    auto y = c.mk_var("y", k);

    SECTION("same_var_eq") {
      REQUIRE(*a == *a);
    }

    SECTION("same_var_name_eq") {
      REQUIRE(*a1 == *a2);
    }

    SECTION("diff_var_neq") {
      REQUIRE(*a != *b);
    }

    SECTION("same_var_name_not_neq") {
      REQUIRE(!(*a1 != *a2));
    }

    SECTION("same_ap_eq") {
      auto fa = c.mk_ap(f, a);
      REQUIRE(*fa == *fa);
    }

    SECTION("diff_ap_neq") {
      auto fa = c.mk_ap(f, a);
      REQUIRE(*fa != *b);
    }

    SECTION("diff_lam_neq") {
      auto fx = c.mk_ap(f, x);
      auto lfx = c.mk_lam(x, fx);
      auto ly = c.mk_lam(x, y);
      REQUIRE(*lfx != *ly);
    }

    SECTION("same_lam_eq") {
      auto fx = c.mk_ap(f, x);
      auto lfx1 = c.mk_lam(x, fx);
      auto lfx2 = c.mk_lam(x, fx);
      REQUIRE(*lfx1 == *lfx2);
    }

    SECTION("lam_free_vars") {
      auto ft = c.mk_tfunc(k, c.mk_tfunc(k, k));
      auto x1 = c.mk_var("x", k);
      auto x2 = c.mk_var("x", k);
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
  
}
