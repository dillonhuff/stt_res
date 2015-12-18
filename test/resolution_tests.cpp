#define CATCH_CONFIG_MAIN

#include <vector>

#include "src/context.h"
#include "src/resolution.h"
#include "test/catch.hpp"
#include "test/resolution_tests.h"

using namespace std;

namespace stt_res {

  TEST_CASE("resolve_empty") {
    context c;
    vector<proof*> clauses;
    auto p = resolve(c, clauses);
    REQUIRE(p == nullptr);
  }

  TEST_CASE("resolve_empty_clause") {
    context c;
    vector<proof*> clauses{c.mk_assumption(empty_clause)};
    auto p = resolve(c, clauses);
    REQUIRE(p->result == empty_clause);
  }

  TEST_CASE("resolve_a_not_a") {
    context c;
    auto a = c.mk_con("a", c.b());
    auto not_a = c.mk_not(a);
    auto a_p = c.mk_assumption(a);
    auto not_a_p = c.mk_assumption(not_a);
    cout << *a << endl;
    cout << *not_a << endl;
    vector<proof*> clauses{a_p, not_a_p};
    auto p = resolve(c, clauses);
    REQUIRE(p->result == empty_clause);
  }

  TEST_CASE("resolve_a_na_one_clause") {
    context c;
    auto a = c.mk_con("a", c.b());
    auto not_a = c.mk_not(a);
    auto a_p = c.mk_assumption(a);
    auto not_a_p = c.mk_assumption(not_a);
    cout << *a << endl;
    cout << *not_a << endl;
    vector<proof*> clauses{a_p, not_a_p};
    auto p = resolve(c, clauses);
    REQUIRE(p->result == empty_clause);
  }

  // TEST_CASE("refute_not_excluded_middle") {
  //   context c;
  //   auto a = c.mk_var("a", c.b());
  //   auto not_a = c.mk_not(a);
  //   auto exc_middle = c.mk_forall(a, c.mk_and(a, not_a));
  //   cout << *exc_middle << endl;
  //   auto exc_middle_p = c.mk_assumption(exc_middle);
  //   vector<proof*> clauses{exc_middle_p};
  //   auto p = resolve(c, clauses);
  //   REQUIRE(p->result == empty_clause);
  // }
  
}
