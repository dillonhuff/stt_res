#include <iostream>

#include "src/ast.h"
#include "src/context.h"
#include "test/catch.hpp"

using namespace std;

namespace stt_res {

  TEST_CASE("context", "[context]") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto z = c.mk_var("z", t);    
    auto ft = c.mk_tfunc(t, t);
    auto f = c.mk_var("f", ft);
    auto g = c.mk_var("g", ft);
    auto F = c.mk_con("F", ft);
    
    SECTION("make_var") {
      auto a1 = c.mk_var("a", t);
      auto a2 = c.mk_var("a", t);
      REQUIRE(*a1 == *a2);
    }

    SECTION("dont_sub_var") {
      auto r = c.sub(x, y, z);
      REQUIRE(*r == *z);
    }

    SECTION("sub_var") {
      auto x1 = c.mk_var("x", t);
      auto x2 = c.mk_var("x", t);
      auto r = c.sub(x1, y, x2);
      REQUIRE(*r == *y);
    }

    SECTION("sub_ap") {
      auto x1 = c.mk_var("x", t);
      auto x2 = c.mk_var("x", t);
      auto fx = c.mk_ap(f, x1);
      auto r = c.sub(x2, y, fx);
      auto correct = c.mk_ap(f, y);
      REQUIRE(*r == *correct);
    }

    SECTION("dont_sub_lam") {
      auto l = c.mk_lam(x, x);
      auto r = c.sub(x, y, l);
      REQUIRE(*r == *l);
    }

    SECTION("sub_lam") {
      auto y1 = c.mk_var("y", t);
      auto y2 = c.mk_var("y", t);
      auto l = c.mk_lam(x, y1);
      auto r = c.sub(y2, z, l);
      auto correct = c.mk_lam(x, z);
      REQUIRE(*r == *correct);
    }

    SECTION("already_unified") {
      sub s{tp(x, x)};
      disagreement_set d(s);
      auto res = c.unify(d);
      REQUIRE(res == true);
      REQUIRE(d.size() == 0);
    }

    SECTION("unify_var") {
      sub s{tp(x, y)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sx = c.apply_sub(final_sub, x);
      auto sy = c.apply_sub(final_sub, y);
      REQUIRE(*sx == *sy);
    }

    SECTION("unify_ap") {
      auto fx = c.mk_ap(f, x);
      auto fy = c.mk_ap(f, y);
      sub s{tp(fx, fy)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sfx = c.apply_sub(final_sub, fx);
      auto sfy = c.apply_sub(final_sub, fy);
      REQUIRE(*sfx == *sfy);
    }

    SECTION("unify_func_var") {
      auto f = c.mk_var("f", c.mk_tfunc(t, ft));
      auto g = c.mk_var("g", c.mk_tfunc(t, ft));
      auto lf = c.mk_lam(x, c.mk_lam(y, c.mk_ap(c.mk_ap(f, x), y)));
      auto lg = c.mk_lam(x, c.mk_lam(y, c.mk_ap(c.mk_ap(g, x), y)));
      sub s{tp(lf, lg)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto slf = c.apply_sub(final_sub, lf);
      auto slg = c.apply_sub(final_sub, lg);
      REQUIRE(*slf == *slg);
    }

    SECTION("unify_2") {
      auto a = c.mk_var("a", t);
      auto gx = c.mk_ap(g, x);
      auto fa = c.mk_ap(f, a);
      auto lgx = c.mk_lam(y, gx);
      sub s{tp(f, lgx), tp(fa, gx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sfa = c.apply_sub(final_sub, fa);
      auto sgx = c.apply_sub(final_sub, gx);
      REQUIRE(*sfa == *sgx);
    }

    SECTION("unify_funcs") {
      auto fx = c.mk_ap(f, x);
      auto gx = c.mk_ap(g, x);
      sub s{tp(fx, gx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sfx = c.apply_sub(final_sub, fx);
      auto sgx = c.apply_sub(final_sub, gx);
      REQUIRE(*sfx == *sgx);
    }

    SECTION("unify_lam_funcs") {
      auto fx = c.mk_ap(f, x);
      auto gx = c.mk_ap(g, x);
      auto lfx = c.mk_lam(x, fx);
      auto lgx = c.mk_lam(x, gx);
      sub s{tp(lfx, lgx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto slfx = c.apply_sub(final_sub, lfx);
      auto slgx = c.apply_sub(final_sub, lgx);
      REQUIRE(*slfx == *slgx);
    }

    SECTION("unify_funcs_multi_var") {
      auto f = c.mk_var("f", c.mk_tfunc(t, ft));
      auto g = c.mk_var("g", c.mk_tfunc(t, ft));      
      auto fxy = c.mk_ap(c.mk_ap(f, x), y);
      auto gxy = c.mk_ap(c.mk_ap(g, x), y);
      cout << "-- " << *gxy << endl;
      cout << "-- " << *fxy << endl;
      sub s{tp(fxy, gxy)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sfx = c.apply_sub(final_sub, fxy);
      auto sgx = c.apply_sub(final_sub, gxy);
      cout << "-- sfx " << *sfx << endl;
      cout << "-- sgx " << *sgx << endl;
      REQUIRE(*sfx == *sgx);
    }

    SECTION("unify_func_const") {
      auto fFx = c.mk_ap(f, c.mk_ap(F, x));
      auto Ffx = c.mk_ap(F, c.mk_ap(f, x));
      cout << *fFx << endl;
      cout << *Ffx << endl;
      sub s{tp(fFx, Ffx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sFfx = c.apply_sub(final_sub, Ffx);
      auto sfFx = c.apply_sub(final_sub, fFx);
      REQUIRE(*sFfx == *sfFx);
    }

    SECTION("unify_fx_Fx") {
      auto fx = c.mk_ap(f, x);
      auto Fx = c.mk_ap(F, x);
      sub s{tp(fx, Fx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sFx = c.apply_sub(final_sub, Fx);
      auto sfx = c.apply_sub(final_sub, fx);
      REQUIRE(*sFx == *sfx);
    }

    SECTION("unify_Fx_Fx") {
      auto Fx = c.mk_ap(F, x);
      sub s{tp(Fx, Fx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sFx = c.apply_sub(final_sub, Fx);
      REQUIRE(*sFx == *sFx);
    }

    SECTION("unify_Fx_Fy") {
      auto Fx = c.mk_ap(F, x);
      auto Fy = c.mk_ap(F, y);
      sub s{tp(Fx, Fy)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto sFx = c.apply_sub(final_sub, Fx);
      auto sFy = c.apply_sub(final_sub, Fy);
      REQUIRE(*sFx == *sFy);
    }

    SECTION("unify_lFx_lfx") {
      auto lFx = c.mk_lam(x, c.mk_ap(F, x));
      auto lfx = c.mk_lam(x, c.mk_ap(f, x));
      sub s{tp(lfx, lFx)};
      disagreement_set d(s);
      c.unify(d);
      auto final_sub = d.extract_sub();
      auto slFx = c.apply_sub(final_sub, lFx);
      auto slfx = c.apply_sub(final_sub, lfx);
      REQUIRE(*slFx == *slfx);
    }
  }

}
