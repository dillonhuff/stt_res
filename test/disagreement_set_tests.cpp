#include "src/context.h"
#include "src/disagreement_set.h"
#include "test/catch.hpp"

namespace stt_res {
  TEST_CASE("disagreement_set", "[disagreement_set]") {
    context c;
    auto t = c.mk_tcon("t");
    auto ft = c.mk_tfunc(t, t);    
    auto x = c.mk_var("x", t);
    auto y = c.mk_var("y", t);    
    auto x1 = c.mk_var("x", t);
    auto y1 = c.mk_var("y", t);
    auto x2 = c.mk_var("x", t);
    auto y2 = c.mk_var("y", t);
    auto f = c.mk_var("f", ft);

    SECTION("insert_var") {
      tp s{x, x};
      disagreement_set d;
      d.insert(s);
      REQUIRE(d.size() == 1);
    }

    SECTION("erase_var") {
      tp s{x, x};
      disagreement_set d;
      d.insert(s);
      d.erase_pair(s);
      REQUIRE(d.size() == 0);
    }

    SECTION("insert_twice") {
      tp s1{x1, y1};
      disagreement_set d;
      d.insert(s1);
      tp s2{x2, y2};    
      d.insert(s2);
      REQUIRE(d.size() == 1);
    }

    SECTION("insert_twice_same_obj") {
      tp s{x1, y1};
      disagreement_set d;
      d.insert(s);
      d.insert(s);
      REQUIRE(d.size() == 1);
    }

    SECTION("insert_different_same_fst") {
      tp s1{x, y};
      disagreement_set d;
      d.insert(s1);
      auto z = c.mk_var("z", t);
      tp s2{x, z};
      d.insert(s2);
      REQUIRE(d.size() == 2);
    }

    SECTION("insert_different_same_snd") {
      tp s1{y, x};
      disagreement_set d;
      d.insert(s1);
      auto z = c.mk_var("z", t);
      tp s2{z, x};
      d.insert(s2);
      REQUIRE(d.size() == 2);
    }
  
    SECTION("insert_different") {
      tp s1{x, y};
      disagreement_set d;
      d.insert(s1);
      auto z = c.mk_var("z", t);
      auto k = c.mk_var("k", t);
      tp s2{z, k};
      d.insert(s2);
      REQUIRE(d.size() == 2);
    }

    SECTION("insert_var_apps") {
      tp s1{c.mk_ap(f, x), x};
      tp s2{c.mk_ap(f, y), y};
      disagreement_set d;
      sub s{s1, s2};
      d.add_pairs(s);
      REQUIRE(d.size() == 2);
    }
  }
  
};
