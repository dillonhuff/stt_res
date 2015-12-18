#include "src/context.h"
#include "src/disagreement_set.h"
#include "test/catch.hpp"
#include "test/disagreement_set_tests.h"
#include "test/test_utils.h"

namespace stt_res {

  TEST_CASE("insert_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    tp s{x, x};
    disagreement_set d;
    d.insert(s);
    REQUIRE(d.size() == 1);
  }

  TEST_CASE("erase_var") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    tp s{x, x};
    disagreement_set d;
    d.insert(s);
    d.erase_pair(s);
    REQUIRE(d.size() == 0);
  }

  TEST_CASE("insert_twice") {
    context c;
    auto t = c.mk_tvar("t");
    auto x1 = c.mk_var("x", t);
    auto y1 = c.mk_var("y", t);
    tp s1{x1, y1};
    disagreement_set d;
    d.insert(s1);
    auto x2 = c.mk_var("x", t);
    auto y2 = c.mk_var("y", t);
    tp s2{x2, y2};    
    d.insert(s2);
    REQUIRE(d.size() == 1);
  }

  TEST_CASE("insert_twice_same_obj") {
    context c;
    auto t = c.mk_tvar("t");
    auto x1 = c.mk_var("x", t);
    auto y1 = c.mk_var("y", t);
    tp s{x1, y1};
    disagreement_set d;
    d.insert(s);
    d.insert(s);
    REQUIRE(d.size() == 1);
  }

  TEST_CASE("insert_different_same_fst") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    tp s1{x, y};
    disagreement_set d;
    d.insert(s1);
    auto z = c.mk_var("z", t);
    tp s2{x, z};
    d.insert(s2);
    REQUIRE(d.size() == 2);
  }

  TEST_CASE("insert_different_same_snd") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    tp s1{y, x};
    disagreement_set d;
    d.insert(s1);
    auto z = c.mk_var("z", t);
    tp s2{z, x};
    d.insert(s2);
    REQUIRE(d.size() == 2);
  }
  
  TEST_CASE("insert_different") {
    context c;
    auto t = c.mk_tvar("t");
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    tp s1{x, y};
    disagreement_set d;
    d.insert(s1);
    auto z = c.mk_var("z", t);
    auto k = c.mk_var("k", t);
    tp s2{z, k};
    d.insert(s2);
    REQUIRE(d.size() == 2);
  }

  TEST_CASE("insert_var_apps") {
    context c;
    auto t = c.mk_tvar("t");
    auto ft = c.mk_tfunc(t, t);
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);
    auto f = c.mk_var("f", ft);
    tp s1{c.mk_ap(f, x), x};
    tp s2{c.mk_ap(f, y), y};
    disagreement_set d;
    sub s{s1, s2};
    d.add_pairs(s);
    REQUIRE(d.size() == 2);
  }
  
};
