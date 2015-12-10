#ifndef STT_RES_CONTEXT_H
#define STT_RES_CONTEXT_H

#include <cassert>
#include <string>
#include <vector>

#include "src/ast.h"

using namespace std;

#define UNIFY_SUCCEEDED 0
#define UNIFY_FAILED 1

namespace stt_res {

  typedef pair<const term*, const term*> tp;
  typedef vector<tp> sub;
  typedef int res_code;

  using namespace stt_res;
  
  class context {
  protected:
    bool system_is_solved(stt_res::sub& x);
    void delete_identical_pairs(stt_res::sub& x);
    bool pair_is_solved(const term* l, const term* r);
    void reduce_pair_args(stt_res::sub& s);
    stt_res::sub reduce_args(const term* l, const term* r);
    const term* append_lambdas(vector<const var*> vars, const term* t);

    
  public:

    const var* mk_var(string name) { return new var(name); }
    const ap* mk_ap(const term* l, const term* r) { return new ap(l, r); }
    const lam* mk_lam(const var* v, const term* e) { return new lam(v, e); }

    const term* sub(const var* target, const term* replacement, const term* t);

    res_code unify(stt_res::sub& s);

    const term* apply_sub(stt_res::sub& s, const term* t);

  };

}

#endif
