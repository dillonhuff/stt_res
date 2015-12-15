#include <iostream>

#include "src/ast.h"
#include "src/context.h"
#include "test/context_tests.h"
#include "test/test_utils.h"

using namespace std;

namespace stt_res {

  void make_var() {
    context c;
    auto t = c.mk_tvar("t");
    auto a1 = c.mk_var("a", t);
    auto a2 = c.mk_var("a", t);
    test_assertion(*a1 == *a2, "make_var");
  }

  void dont_sub_var() {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto z = c.mk_var("z", t);
    auto r = c.sub(x, y, z);
    test_assertion(*r == *z, "dont_sub_var");
  }

  void sub_var() {
    context c;
    auto t = c.mk_tvar("t");
    auto x1 = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto x2 = c.mk_var("x", t);
    auto r = c.sub(x1, y, x2);
    test_assertion(*r == *y, "sub_var");
  }

  void sub_ap() {
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
    test_assertion(*r == *correct, "sub_ap");
  }

  void dont_sub_lam() {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto l = c.mk_lam(x, x);
    auto r = c.sub(x, y, l);
    test_assertion(*r == *l, "dont_sub_lam");
  }

  void sub_lam() {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y1 = c.mk_var("y", t);
    auto y2 = c.mk_var("y", t);
    auto z = c.mk_var("z", t);
    auto l = c.mk_lam(x, y1);
    auto r = c.sub(y2, z, l);
    auto correct = c.mk_lam(x, z);
    test_assertion(*r == *correct, "sub_lam");
  }

  void already_unified() {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    sub s{tp(x, x)};
    disagreement_set d(s);
    auto res = c.unify(d);
    test_assertion(res == UNIFY_SUCCEEDED && d.size() == 0, "already_unified");
  }

  void unify_var() {
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
    test_assertion(*sx == *sy, "unify_var");
  }

  void unify_ap() {
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
    test_assertion(*sfx == *sfy, "unify_ap");
  }

  void unify_func_var() {
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
    test_assertion(*slf == *slg, "unify_func_var");
  }

  void unify_2() {
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
    test_assertion(*sfa == *sgx, "unify_2");
  }

  void unify_funcs() {
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
    test_assertion(*sfx == *sgx, "unify_funcs");
  }

  void unify_lam_funcs() {
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
    test_assertion(*slfx == *slgx, "unify_lam_funcs");
  }

  void unify_funcs_multi_var() {
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
    test_assertion(*sfx == *sgx, "unify_funcs_multi_var");
  }

  void unify_func_const() {
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
    test_assertion(*sFfx == *sfFx, "unify_func_const");
  }

  void unify_fx_Fx() {
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
    test_assertion(*sFx == *sfx, "unify_fx_Fx");
  }

  void unify_Fx_Fx() {
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
    test_assertion(*sFx == *sFx, "unify_Fx_Fx");
  }

  void unify_Fx_Fy() {
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
    test_assertion(*sFx == *sFy, "unify_Fx_Fy");
  }

  void unify_lFx_lfx() {
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
    test_assertion(*slFx == *slfx, "unify_lFx_lfx");
  }  
  
  void all_context_tests() {
    make_var();
    dont_sub_var();
    sub_var();
    sub_ap();
    dont_sub_lam();
    sub_lam();
    unify_var();
    already_unified();
    unify_ap();
    unify_func_var();
    unify_2();
    unify_funcs();
    unify_lam_funcs();
    unify_funcs_multi_var();
    unify_Fx_Fx();
    unify_Fx_Fy();
    unify_lFx_lfx();
    unify_fx_Fx();
    //unify_func_const();
  }

}
