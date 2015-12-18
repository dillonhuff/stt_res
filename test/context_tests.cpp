#include <iostream>

#include "src/ast.h"
#include "src/context.h"
#include "test/catch.hpp"

using namespace std;

namespace stt_res {

  TEST_CASE("make_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto a1 = c.mk_var("a", t);
    auto a2 = c.mk_var("a", t);
    REQUIRE(*a1 == *a2);
  }

  TEST_CASE("dont_sub_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto z = c.mk_var("z", t);
    auto r = c.sub(x, y, z);
    REQUIRE(*r == *z);
  }

  TEST_CASE("sub_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto x1 = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto x2 = c.mk_var("x", t);
    auto r = c.sub(x1, y, x2);
    REQUIRE(*r == *y);
  }

  TEST_CASE("sub_ap") {
    context c;
    auto t = c.mk_tvar("t");
    auto x1 = c.mk_var("x", t);
    auto x2 = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto ft = c.mk_tfunc(t, t);
    auto f = c.mk_var("f", ft);
    auto fx = c.mk_ap(f, x1);
    auto r = c.sub(x2, y, fx);
    auto correct = c.mk_ap(f, y);
    REQUIRE(*r == *correct);
  }

  TEST_CASE("dont_sub_lam") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto l = c.mk_lam(x, x);
    auto r = c.sub(x, y, l);
    REQUIRE(*r == *l);
  }

  TEST_CASE("sub_lam") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y1 = c.mk_var("y", t);
    auto y2 = c.mk_var("y", t);
    auto z = c.mk_var("z", t);
    auto l = c.mk_lam(x, y1);
    auto r = c.sub(y2, z, l);
    auto correct = c.mk_lam(x, z);
    REQUIRE(*r == *correct);
  }

  TEST_CASE("already_unified") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    sub s{tp(x, x)};
    disagreement_set d(s);
    auto res = c.unify(d);
    REQUIRE(res == true);
    REQUIRE(d.size() == 0);
  }

  TEST_CASE("unify_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    sub s{tp(x, y)};
    disagreement_set d(s);
    c.unify(d);
    auto final_sub = d.extract_sub();
    auto sx = c.apply_sub(final_sub, x);
    auto sy = c.apply_sub(final_sub, y);
    REQUIRE(*sx == *sy);
  }

  TEST_CASE("unify_ap") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto f = c.mk_var("f", ft);
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

  TEST_CASE("unify_func_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, c.mk_tfunc(t, t));
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto f = c.mk_var("f", ft);
    auto g = c.mk_var("g", ft);
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

  TEST_CASE("unify_2") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto a = c.mk_var("a", t);
    auto f = c.mk_var("f", ft);
    auto g = c.mk_var("g", ft);
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

  TEST_CASE("unify_funcs") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto f = c.mk_var("f", ft);
    auto g = c.mk_var("g", ft);
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

  TEST_CASE("unify_lam_funcs") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto f = c.mk_var("f", ft);
    auto g = c.mk_var("g", ft);
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

  TEST_CASE("unify_funcs_multi_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, c.mk_tfunc(t, t));
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto f = c.mk_var("f", ft);
    auto g = c.mk_var("g", ft);
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

  TEST_CASE("unify_func_const") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto f = c.mk_var("f", ft);
    auto F = c.mk_con("F", ft);
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

  TEST_CASE("unify_fx_Fx") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto f = c.mk_var("f", ft);
    auto F = c.mk_con("F", ft);
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

  TEST_CASE("unify_Fx_Fx") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto F = c.mk_con("F", ft);
    auto Fx = c.mk_ap(F, x);
    sub s{tp(Fx, Fx)};
    disagreement_set d(s);
    c.unify(d);
    auto final_sub = d.extract_sub();
    auto sFx = c.apply_sub(final_sub, Fx);
    REQUIRE(*sFx == *sFx);
  }

  TEST_CASE("unify_Fx_Fy") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto F = c.mk_con("F", ft);
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

  TEST_CASE("unify_lFx_lfx") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto F = c.mk_con("F", ft);
    auto f = c.mk_var("f", ft);
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
