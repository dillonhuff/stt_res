#include <iostream>

#include "src/ast.h"
#include "src/context.h"
#include "test/context_tests.h"
#include "test/test_utils.h"

using namespace std;

namespace stt_res {

  void make_var() {
    context c;
    auto a1 = c.mk_var("a");
    auto a2 = c.mk_var("a");
    test_assertion(*a1 == *a2, "make_var");
  }

  void dont_sub_var() {
    context c;
    auto x = c.mk_var("x");
    auto y = c.mk_var("y");
    auto z = c.mk_var("z");
    auto r = c.sub(x, y, z);
    test_assertion(*r == *z, "dont_sub_var");
  }

  void sub_var() {
    context c;
    auto x1 = c.mk_var("x");
    auto y = c.mk_var("y");
    auto x2 = c.mk_var("x");
    auto r = c.sub(x1, y, x2);
    test_assertion(*r == *y, "sub_var");
  }

  void sub_ap() {
    context c;
    auto x1 = c.mk_var("x");
    auto x2 = c.mk_var("x");
    auto y = c.mk_var("y");
    auto f = c.mk_var("f");
    auto fx = c.mk_ap(f, x1);
    auto r = c.sub(x2, y, fx);
    auto correct = c.mk_ap(f, y);
    test_assertion(*r == *correct, "sub_ap");
  }

  void dont_sub_lam() {
    context c;
    auto x = c.mk_var("x");
    auto y = c.mk_var("y");
    auto l = c.mk_lam(x, x);
    auto r = c.sub(x, y, l);
    test_assertion(*r == *l, "dont_sub_lam");
  }

  void sub_lam() {
    context c;
    auto x = c.mk_var("x");
    auto y1 = c.mk_var("y");
    auto y2 = c.mk_var("y");
    auto z = c.mk_var("z");
    auto l = c.mk_lam(x, y1);
    auto r = c.sub(y2, z, l);
    auto correct = c.mk_lam(x, z);
    test_assertion(*r == *correct, "sub_lam");
  }

  void unify_var() {
    context c;
    auto x = c.mk_var("x");
    sub s{tp(x, x)};
    auto res = c.unify(s);
    test_assertion(res == UNIFY_SUCCEEDED && s.size() == 0, "unify_var");
  }

  void already_unified() {
    context c;
    auto x = c.mk_var("x");
    auto y = c.mk_var("y");
    sub s{tp(x, y)};
    c.unify(s);
    auto sx = c.apply_sub(s, x);
    auto sy = c.apply_sub(s, y);
    test_assertion(*sx == *sy, "already_unified");
  }

  void unify_ap() {
    context c;
    auto x = c.mk_var("x");
    auto y = c.mk_var("y");
    auto f = c.mk_var("f");
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
    auto x = c.mk_var("x");
    auto y = c.mk_var("y");
    auto f = c.mk_var("f");
    auto g = c.mk_var("g");
    auto lf = c.mk_lam(x, c.mk_lam(y, c.mk_ap(c.mk_ap(f, x), y)));
    auto lg = c.mk_lam(x, c.mk_lam(y, c.mk_ap(c.mk_ap(g, x), y)));
    cout << "-- " << *lf << endl;
    cout << "-- " << *lg << endl;
    sub s{tp(lf, lg)};
    c.unify(s);
    auto slf = c.apply_sub(s, lf);
    auto slg = c.apply_sub(s, lg);
    cout << "-- " << *slf << endl;
    cout << "-- " << *slg << endl;
    test_assertion(*slf == *slg, "unify_func_var");
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
  }

}
