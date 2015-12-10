#include "src/context.h"

namespace stt_res {

  const term* context::sub(const var* target, const term* replacement, const term* t) {
    if (t->is_var()) {
      auto v = static_cast<const var*>(t);
      if (*v == *target) {
	return replacement;
      } else {
	return t;
      }
    }

    if (t->is_ap()) {
      auto a = static_cast<const ap*>(t);
      auto newl = sub(target, replacement, a->l);
      auto newr = sub(target, replacement, a->r);
      return mk_ap(newl, newr);
    }

    if (t->is_lam()) {
      auto l = static_cast<const lam*>(t);
      if (*(l->v) == *target) {
	return t;
      } else {
	auto fvs = free_vars(replacement);
	auto val = *(l->v);
	if (count_if(fvs.begin(), fvs.end(), [val](const var* x) {return val == *x;}) == 0) {
	  return mk_lam(l->v, sub(target, replacement, l->e));
	} else {
	  assert(false);
	}
      }
    }
    assert(false);
  }

  bool context::pair_is_solved(const term* l, const term* r) {
    if (l->is_var()) {
      auto fvs = free_vars(r);
      auto vr = static_cast<const var*>(l);
      auto val = *vr;
      return !(count_if(fvs.begin(), fvs.end(), [val](const var* v){return val == *v;}));
    }
    return false;
  }

  bool context::system_is_solved(stt_res::sub& s) {
    auto num_solved_pairs = count_if(s.begin(), s.end(), [this](tp p) {return pair_is_solved(p.first, p.second);});
    auto num_pairs = s.size();
    return num_solved_pairs == num_pairs;
  }

  void context::delete_identical_pairs(stt_res::sub& s) {
    s.erase(remove_if(s.begin(), s.end(), [](tp p) {return *(p.first) == *(p.second);}), s.end());
  }

  stt_res::sub context::reduce_args(const term* l, const term* r) {
    stt_res::sub s;
    auto ls = split_leading_lambdas(l);
    auto rs = split_leading_lambdas(r);
    if (vars_equal(ls.first, rs.first)) {
      auto la = split_args(ls.second);
      auto ra = split_args(rs.second);
      if (*(la.first) == *(ra.first)) {
    	auto largs = la.second;
    	auto rargs = ra.second;
    	assert(largs.size() == rargs.size());
    	for (int i = 0; i < largs.size(); i++) {
    	  auto newArgL = append_lambdas(ls.first, largs[i]);
    	  auto newArgR = append_lambdas(rs.first, rargs[i]);
    	  s.push_back(tp(newArgL, newArgR));
    	}
      }
    }
    return s;
  }

  void context::reduce_pair_args(stt_res::sub& s) {
    for (auto p : s) {
      auto subpairs = reduce_args(p.first, p.second);
      if (subpairs.size() > 0) {
	s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.first) && *(r.second) == *(p.second); }), s.end());
	s.insert(s.end(), subpairs.begin(), subpairs.end());
      }
    }
  }

  // TODO: Fix this horrible use of iterators. Really should be
  // filtering and then applying solver
  void context::solve_vars(stt_res::sub& s) {
    for (auto p : s) {
      auto ls = split_leading_lambdas(p.first);
      auto rs = split_leading_lambdas(p.second);
      if (vars_equal(ls.first, rs.first)) {
	auto la = split_args(ls.second);
	if (la.first->is_var()) {
	  auto val = static_cast<const var*>(la.first);
	  auto fvs = free_vars(p.second);
	  if (count_if(fvs.begin(), fvs.end(), [val](const var* v) {return *v == *val;}) == 0) {
	    s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.first) && *(r.second) == *(p.second); }), s.end());
	    for (auto p2 : s) {
	    }
	    s.push_back(tp(val, p.second));
	  }
	}
      }
    }
  }

  res_code context::unify(stt_res::sub& s) {
    while (true) {
      if (system_is_solved(s)) {
	return UNIFY_SUCCEEDED;
      }
      auto v = s;
      delete_identical_pairs(s);
      reduce_pair_args(s);
      solve_vars(s);
      if (v == s) {
	return UNIFY_FAILED;
      }
    }
  }

  const term* context::beta_reduce(const term* t) {
    if (t->is_var()) {
      return t;
    }
    if (t->is_lam()) {
      auto t_lam = static_cast<const lam*>(t);
      return mk_lam(t_lam->v, beta_reduce(t_lam->e));
    }
    if (t->is_ap()) {
      auto t_ap = static_cast<const ap*>(t);
      auto br_l = beta_reduce(t_ap->l);
      auto br_r = beta_reduce(t_ap->r);
      if (br_l->is_lam()) {
	auto br_l_lam = static_cast<const lam*>(br_l);
	return sub(br_l_lam->v, br_r, br_l_lam->e);
      }
      return mk_ap(br_l, br_r);
    }
    assert(false);
  }

  const term* context::apply_sub(stt_res::sub& s, const term* t) {
    auto t_loc = &t;
    for (auto p : s) {
      assert(p.first->is_var());
      auto v = static_cast<const var*>(p.first);
      auto e = p.second;
      auto new_t = sub(v, e, *t_loc);
      t_loc = &new_t;
    }
    auto br = beta_reduce(*t_loc);
    return br;
  }

  const term* context::append_lambdas(vector<const var*> vars, const term* t) {
    const term* const* t_loc = &t;
    for (auto rit = vars.rbegin(); rit != vars.rend(); ++rit) {
      auto v = *rit;
      auto new_lam = static_cast<const term*>(mk_lam(v, t));
      t_loc = &new_lam;
    }
    return *t_loc;
  }

}
