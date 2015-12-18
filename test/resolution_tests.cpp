#define CATCH_CONFIG_MAIN

#include <vector>

#include "src/context.h"
#include "src/resolution.h"
#include "test/catch.hpp"

using namespace std;

namespace stt_res {

  TEST_CASE("resolution", "[resolution]") {
    context c;
    auto a = c.mk_con("a", c.b());
    auto not_a = c.mk_not(a);
    auto a_p = c.mk_assumption(a);
    auto not_a_p = c.mk_assumption(not_a);
    cout << *a << endl;
    cout << *not_a << endl;
    
    SECTION("resolve_empty") {
      vector<proof*> clauses;
      auto p = resolve(c, clauses);
      REQUIRE(p == nullptr);
    }

    SECTION("resolve_empty_clause") {
      vector<proof*> clauses{c.mk_assumption(empty_clause)};
      auto p = resolve(c, clauses);
      REQUIRE(p->result == empty_clause);
    }

    SECTION("resolve_a_not_a") {
      vector<proof*> clauses{a_p, not_a_p};
      auto p = resolve(c, clauses);
      REQUIRE(p->result == empty_clause);
    }

    // TODO: Change to (~ a) \/ a
    SECTION("resolve_a_na_one_clause") {
      vector<proof*> clauses{a_p, not_a_p};
      auto p = resolve(c, clauses);
      REQUIRE(p->result == empty_clause);
    }

    SECTION("refute_not_excluded_middle") {
      auto a = c.mk_var("a", c.b());
      auto not_a = c.mk_not(a);
      auto exc_middle = c.mk_forall(a, c.mk_and(a, not_a));
      cout << *exc_middle << endl;
      auto exc_middle_p = c.mk_assumption(exc_middle);
      vector<proof*> clauses{exc_middle_p};
      auto p = resolve(c, clauses);
      REQUIRE(p->result == empty_clause);
    }

  }
  
}
