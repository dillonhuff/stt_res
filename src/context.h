#ifndef STT_RES_CONTEXT_H
#define STT_RES_CONTEXT_H

#include <cassert>
#include <string>
#include <vector>

#include "src/arena_allocator.h"
#include "src/assumption.h"
#include "src/ast.h"
#include "src/disagreement_set.h"
#include "src/proof.h"
#include "src/res.h"

using namespace std;

namespace stt_res {

  typedef int res_code;

  using namespace stt_res;
  
  class context {
  protected:
    int next_unique_num;

    pair<const var*, tp> pair_is_solvable(tp p);
    pair<const var*, tp> term_solvable(const term* l, const term* r);
    

    bool unify_dfs(disagreement_set& s, int depth);
    stt_res::sub reduce_args(const term* l, const term* r);
    const term* append_lambdas(vector<const var*> vars, const term* t);
    const term* apply_args(const term* t, vector<const term*> args);
    
    pair<const var*, tp> find_solvable_pair(stt_res::disagreement_set& s);
    
    vector<const term*> projection_bindings(const term* t);

    const term* imitation_binding(const term* a, const term* f);
    vector<const term*> inner_imitation_binding_args(vector<const var*> ys, const term* a);
    vector<const var*> outer_imitation_binding_args(const term* f);
    const term* inner_imitation_binding_arg(vector<const var*> ys, const type* t);

    arena_allocator allocator;
    const tvar* boolean;

  public:
    void reduce_pair_args(stt_res::disagreement_set& s);    
    void delete_identical_pairs(stt_res::disagreement_set& x);
    bool solve_vars(stt_res::disagreement_set& s);
    bool add_projection_binding(stt_res::disagreement_set& s);
    bool add_imitation_binding(stt_res::disagreement_set& s);
    
    context() {
      next_unique_num = 0;
      boolean = mk_tvar("b");
    }

    const tvar* b() { return boolean; }

    const term* mk_not(const term* t) {
      auto ptr = static_cast<con*>(allocator.allocate(sizeof(con)));
      auto nf = mk_tfunc(b(), b());
      auto neg = new (ptr) con("~", nf);
      return mk_ap(neg, t);
    }

    proof* mk_assumption(const term* t) {
      auto ptr = static_cast<assumption*>(allocator.allocate(sizeof(assumption)));
      return new (ptr) assumption(t);
    }

    proof* mk_res(stt_res::sub& theta, proof* l, proof* r, const term* t) {
      auto ptr = static_cast<res*>(allocator.allocate(sizeof(res)));
      return new (ptr) res(theta, l, r, t);
    }
    
    const tvar* mk_tvar(string name) {
      auto ptr = static_cast<tvar*>(allocator.allocate(sizeof(tvar)));
      return new (ptr) tvar(name);
    }

    const type* mk_tfunc(const type* l, const type* r) {
      auto ptr = static_cast<tfunc*>(allocator.allocate(sizeof(tfunc)));
      return new (ptr) tfunc(l, r);
    }
    
    const con* mk_con(string name, const type* t) {
      auto ptr = static_cast<con*>(allocator.allocate(sizeof(con)));
      return new (ptr) con(name, t);
    }
    
    const var* mk_var(string name, const type* t) {
      auto ptr = static_cast<var*>(allocator.allocate(sizeof(var)));
      return new (ptr) var(name, t);
    }

    const ap* mk_ap(const term* l, const term* r) {
      auto ptr = static_cast<ap*>(allocator.allocate(sizeof(ap)));
      return new (ptr) ap(l, r);
    }
    
    const lam* mk_lam(const var* v, const term* e) {
      auto t = mk_tfunc(v->t, e->t);
      auto ptr = static_cast<lam*>(allocator.allocate(sizeof(lam)));
      return new (ptr) lam(v, e, t);
    }

    const term* mk_or(const term* l, const term* r) {
      auto ptr = static_cast<con*>(allocator.allocate(sizeof(con)));
      auto nf = mk_tfunc(b(), mk_tfunc(b(), b()));
      auto o = new (ptr) con("or", nf);
      return mk_ap(mk_ap(o, l), r);
    }

    const term* mk_pi(const term* t) {
      auto ptr = static_cast<con*>(allocator.allocate(sizeof(con)));
      //auto a = mk_tvar("a");
      auto nf = mk_tfunc(mk_tfunc(b(), b()), b());
      auto pi = new (ptr) con("pi", nf);
      return mk_ap(pi, t);
    }

    const term* mk_and(const term* l, const term* r) {
      return mk_not(mk_or(l, r));
    }
    
    const term* mk_forall(const var* v, const term* e) {
      return mk_pi(mk_lam(v, e));
    }

    const term* mk_clause(vector<const term*> ts) {
      assert(ts.size() > 0);
      auto t = ts.back();
      if (ts.size() > 1) {
	ts.pop_back();
	auto res = mk_clause(ts);
	return mk_or(t, res);
      }
      return t;
    }

    const term* sub(const var* target, const term* replacement, const term* t);

    bool unify(stt_res::disagreement_set& s);

    const term* apply_sub(stt_res::sub& s, const term* t);

    const term* beta_reduce(const term* t);

    const var* fresh_var(string prefix, const type* t);

  };

  void erase_pair(stt_res::tp p, stt_res::disagreement_set& s);

}

#endif
