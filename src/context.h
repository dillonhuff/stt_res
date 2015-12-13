#ifndef STT_RES_CONTEXT_H
#define STT_RES_CONTEXT_H

#include <cassert>
#include <string>
#include <vector>

#include "src/ast.h"
#include "src/disagreement_set.h"

using namespace std;

#define UNIFY_SUCCEEDED 0
#define UNIFY_FAILED 1

namespace stt_res {

  typedef int res_code;

  using namespace stt_res;
  
  class context {
  protected:
    int next_unique_num;

    pair<const var*, tp> pair_is_solvable(tp p);
    pair<const var*, tp> term_solvable(const term* l, const term* r);
    bool system_is_solved(stt_res::disagreement_set& x);
    void delete_identical_pairs(stt_res::disagreement_set& x);
    bool pair_is_solved(stt_res::disagreement_set& s, const term* l, const term* r);
    void reduce_pair_args(stt_res::disagreement_set& s);
    stt_res::sub reduce_args(const term* l, const term* r);
    const term* append_lambdas(vector<const var*> vars, const term* t);
    const term* apply_args(const term* t, vector<const term*> args);
    void solve_vars(stt_res::disagreement_set& s);
    pair<const var*, tp> find_solvable_pair(stt_res::disagreement_set& s);
    void add_imitation_binding(stt_res::disagreement_set& s);
    const term* imitation_binding(const term* a, const term* f);
    vector<const term*> inner_imitation_binding_args(vector<const var*> ys, const term* a);
    vector<const var*> outer_imitation_binding_args(const term* f);
    const term* inner_imitation_binding_arg(vector<const var*> ys, const type* t);
    
    
  public:

    context() {
      next_unique_num = 0;
    }
    
    const type* mk_tvar(string name) {
      auto v = new tvar(name);
      return v;
    }
    
    const type* mk_tfunc(const type* l, const type* r) { return new tfunc(l, r); }
    const var* mk_var(string name, const type* t) {
      auto v = new var(name, t);
      return v;
    }
    const ap* mk_ap(const term* l, const term* r) { return new ap(l, r); }
    const lam* mk_lam(const var* v, const term* e) { return new lam(v, e); }

    const term* sub(const var* target, const term* replacement, const term* t);

    res_code unify(stt_res::disagreement_set& s);

    const term* apply_sub(stt_res::sub& s, const term* t);

    const term* beta_reduce(const term* t);

    const var* fresh_var(string prefix, const type* t);

  };

  void erase_pair(stt_res::tp p, stt_res::disagreement_set& s);

  void delete_duplicates(stt_res::disagreement_set& s);  

}

#endif
