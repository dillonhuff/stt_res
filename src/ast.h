#ifndef STT_RES_AST_H
#define STT_RES_AST_H

#include <set>
#include <vector>

#include "src/ap.h"
#include "src/lam.h"
#include "src/tfunc.h"
#include "src/tvar.h"
#include "src/var.h"

namespace stt_res {

  struct term_comparator {
    bool operator()(const term* l, const term* r) {
      return false;
    }
  };
  
  typedef pair<const term*, const term*> tp;

  struct tp_comparator {
    bool operator()(tp l, tp r) {
      term_comparator t;
      return t(l.first, r.first);
    }
  };
    
  typedef vector<tp> sub;
  typedef std::set<tp, tp_comparator> dset;

  vector<const var*> free_vars(const term* t);

  pair<vector<const var*>, const term*> split_leading_lambdas(const term* t);

  pair<const term*, vector<const term*>> split_args(const term* t);

  const term* append_lambdas(vector<const var*> vars, const term* t);

  bool vars_equal(vector<const var*> l, vector<const var*> r);

  bool free_in(const var* v, const term* t);

  pair<vector<const var*>, pair<const term*, const term*>> match_lambdas(const term* l, const term* r);
}
#endif
