#ifndef STT_RES_CONTEXT_H
#define STT_RES_CONTEXT_H

#include <cassert>
#include <string>
#include <vector>

#include "src/arena_allocator.h"
#include "src/ast.h"
#include "src/disagreement_set.h"

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

  public:
    void reduce_pair_args(stt_res::disagreement_set& s);    
    void delete_identical_pairs(stt_res::disagreement_set& x);
    bool solve_vars(stt_res::disagreement_set& s);
    bool add_projection_binding(stt_res::disagreement_set& s);
    bool add_imitation_binding(stt_res::disagreement_set& s);
    
    context() {
      next_unique_num = 0;
    }
    
    const type* mk_tvar(string name) {
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

    const term* sub(const var* target, const term* replacement, const term* t);

    bool unify(stt_res::disagreement_set& s);

    const term* apply_sub(stt_res::sub& s, const term* t);

    const term* beta_reduce(const term* t);

    const var* fresh_var(string prefix, const type* t);

  };

  void erase_pair(stt_res::tp p, stt_res::disagreement_set& s);

}

#endif
