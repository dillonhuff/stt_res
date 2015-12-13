#ifndef STT_RES_AST_H
#define STT_RES_AST_H

#include <functional>
#include <set>
#include <unordered_set>
#include <vector>

#include "src/ap.h"
#include "src/lam.h"
#include "src/tfunc.h"
#include "src/tvar.h"
#include "src/var.h"

namespace stt_res {

  struct term_comparator {
    bool operator()(const term* l, const term* r) const {
      if (l->is_ap() && r->is_var()) {
	return false;
      }
      if (l->is_var() && r->is_ap()) {
	return true;
      }
      if (l->is_var() && r->is_var()) {
  	auto l_var = static_cast<const var*>(l);
  	auto r_var = static_cast<const var*>(r);
  	return l_var->name < r_var->name;
      }
      if (l->is_ap() && r->is_ap()) {
	auto l_app = static_cast<const ap*>(l);
	auto r_app = static_cast<const ap*>(r);
	auto lf = l_app->l;
	auto ls = l_app->r;
	auto rf = r_app->l;
	auto rs = r_app->r;
	term_comparator t;
	if (t(lf, rf)) {
	  return true;
	} else if (*lf == *rf) {
	  return t(ls, rs);
	} else {
	  return false;
	}	
      }
      if (l->is_lam() && r->is_lam()) {
	auto l_lam = static_cast<const lam*>(l);
	auto r_lam = static_cast<const lam*>(r);
	auto lv = l_lam->v;
	auto rv = r_lam->v;
	auto le = l_lam->e;
	auto re = r_lam->e;
	term_comparator t;
	if (t(lv, rv)) {
	  return true;
	} else if (*lv == *rv) {
	  return t(le, re);
	} else {
	  return false;
	}
      }
      cout << *l << " , " << *r << endl;
      assert(false);
    }
  };
  
  typedef pair<const term*, const term*> tp;

  struct tp_comparator {
    bool operator()(tp l, tp r) const {
      term_comparator t;
      auto lf = l.first;
      auto ls = l.second;
      auto rf = r.first;
      auto rs = r.second;
      if (t(lf, rf)) {
	return true;
      } else if (*lf == *rf) {
	return t(ls, rs);
      } else {
	return false;
      }
    }
  };
    
  typedef vector<tp> sub;
  typedef std::set<tp, tp_comparator> dset;
  typedef std::unordered_set<tp> dset_un;

  vector<const var*> free_vars(const term* t);

  pair<vector<const var*>, const term*> split_leading_lambdas(const term* t);

  pair<const term*, vector<const term*>> split_args(const term* t);

  const term* append_lambdas(vector<const var*> vars, const term* t);

  bool vars_equal(vector<const var*> l, vector<const var*> r);

  bool free_in(const var* v, const term* t);

  pair<vector<const var*>, pair<const term*, const term*>> match_lambdas(const term* l, const term* r);
}
#endif
