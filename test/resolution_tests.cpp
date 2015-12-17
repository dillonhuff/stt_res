#include <vector>

#include "src/context.h"
#include "src/resolution.h"
#include "test/resolution_tests.h"
#include "test/test_utils.h"

using namespace std;

namespace stt_res {

  void resolve_empty() {
    context c;
    vector<proof*> clauses;
    auto p = resolve(c, clauses);
    test_assertion(p == nullptr, "resolve_empty");
  }

  void resolve_empty_clause() {
    context c;
    vector<proof*> clauses{c.mk_assumption(empty_clause)};
    auto p = resolve(c, clauses);
    test_assertion(p->result == empty_clause, "resolve_empty_clause");
  }

  void resolve_a_not_a() {
    context c;
    auto a = c.mk_var("a", c.b());
    auto not_a = c.mk_not(a);
    auto a_p = c.mk_assumption(a);
    auto not_a_p = c.mk_assumption(not_a);
    cout << *a << endl;
    cout << *not_a << endl;
    vector<proof*> clauses{a_p, not_a_p};
    auto p = resolve(c, clauses);
    test_assertion(p->result == empty_clause, "resolve_a_not_a");
  }

  void resolve_a_na_one_clause() {
    context c;
    auto a = c.mk_var("a", c.b());
    auto not_a = c.mk_not(a);
    auto a_p = c.mk_assumption(a);
    auto not_a_p = c.mk_assumption(not_a);
    cout << *a << endl;
    cout << *not_a << endl;
    vector<proof*> clauses{a_p, not_a_p};
    auto p = resolve(c, clauses);
    test_assertion(p->result == empty_clause, "resolve_a_not_a");    
  }
  
  void all_resolution_tests() {
    resolve_empty();
    resolve_empty_clause();
    resolve_a_not_a();
    resolve_a_na_one_clause();
  }
  
}
