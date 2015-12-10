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

  void unify_var() {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    sub s{tp(x, x)};
    auto res = c.unify(s);
    test_assertion(res == UNIFY_SUCCEEDED && s.size() == 0, "unify_var");
  }

  void already_unified() {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    sub s{tp(x, y)};
    c.unify(s);
    auto sx = c.apply_sub(s, x);
    auto sy = c.apply_sub(s, y);
    test_assertion(*sx == *sy, "already_unified");
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
    c.unify(s);
    auto sfx = c.apply_sub(s, fx);
    auto sfy = c.apply_sub(s, fy);
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
    cout << "-- " << *lf << endl;
    cout << "-- " << *lg << endl;    
    sub s{tp(lf, lg)};
    c.unify(s);
    auto slf = c.apply_sub(s, lf);
    auto slg = c.apply_sub(s, lg);
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
    c.unify(s);
    for (auto p : s) {
      cout << *(p.first) << " / " << *(p.second) << endl;
    }    
    auto sfa = c.apply_sub(s, fa);
    auto sgx = c.apply_sub(s, gx);
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
    cout << "-- " << *gx << endl;
    cout << "-- " << *fx << endl;
    sub s{tp(fx, gx)};
    c.unify(s);
    for (auto p : s) {
      cout << *(p.first) << " / " << *(p.second) << endl;
    }
    auto sfx = c.apply_sub(s, fx);
    auto sgx = c.apply_sub(s, gx);
    test_assertion(*sfx == *sgx, "unify_funcs");
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
  }

}
