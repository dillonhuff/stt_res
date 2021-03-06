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

    if (t->is_con()) {
      return t;
    }
    assert(false);
  }

  const var* context::fresh_var(string prefix, const type* t) {
    auto n = "$" + prefix + to_string(next_unique_num);
    next_unique_num++;
    return mk_var(n, t);
  }

  const term* context::apply_args(const term* t, vector<const term*> args) {
    const term* const* t_loc = &t;
    for (auto a : args) {
      auto r = static_cast<const term*>(mk_ap(*t_loc, a));
      t_loc = &r;
    }
    return *t_loc;
  }

  vector<const var*> context::outer_imitation_binding_args(const term* f) {
    //    cout << "-- outer f: " << *f << endl;
    auto tps = f->t->arg_types();
    vector<const var*> arg_vars;
    for (auto t : tps) {
      //      cout << "-- outer tps: " << *t << endl;;
      arg_vars.push_back(fresh_var("y", t));
    }
    return arg_vars;
  }

  const term* context::inner_imitation_binding_arg(vector<const var*> ys,
						   const type* t) {
    auto tps = t->arg_types();
    vector<const term*> all_args;
    for (auto y : ys) {
      all_args.push_back(y);
    }
    vector<const var*> zs;
    for (auto tp : tps) {
      auto v = fresh_var("z", tp);
      all_args.push_back(static_cast<const term*>(v));
      zs.push_back(v);
    }
    const type* const* t_loc = &t;
    for (vector<const term*>::reverse_iterator i = all_args.rbegin(); 
    	 i != all_args.rend(); ++i ) {
      auto ft = mk_tfunc((*i)->t, *t_loc);
      t_loc = &ft;
    }
    auto h_type = *t_loc;
    auto h = fresh_var("H", h_type);
    return append_lambdas(zs, apply_args(h, all_args));
  }

  vector<const term*> context::inner_imitation_binding_args(vector<const var*> ys,
							    const term* a) {
    auto tps = a->t->arg_types();
    vector<const term*> args;
    for (auto t : tps) {
      auto h = inner_imitation_binding_arg(ys, t);
      args.push_back(h);
    }
    return args;
  }

  const term* context::imitation_binding(const term* a, const term* f) {
    auto ys = outer_imitation_binding_args(f);
    auto hs = inner_imitation_binding_args(ys, a);
    auto imitation_binding = append_lambdas(ys, apply_args(a, hs));
    return imitation_binding;
  }

  bool context::add_imitation_binding(disagreement_set& s) {
    tp to_sub;
    auto sub_any = false;
    for (auto p : s) {
      auto left_lam_term = split_leading_lambdas(p.first);
      auto right_lam_term = split_leading_lambdas(p.second);
      if (vars_equal(left_lam_term.first, right_lam_term.first)) {
	auto left_head_and_args = split_args(left_lam_term.second);
	auto right_head_and_args = split_args(right_lam_term.second);
	auto left_head = left_head_and_args.first;
	auto right_head = right_head_and_args.first;
	if (heads_match_imitation(left_head, right_head, p.second)) {
	    sub_any = true;
	    auto t = imitation_binding(right_head, left_head);
	    to_sub = tp(left_head, t);
	}
      }
    }
    if (sub_any) {
      s.insert(to_sub);
      return true;
    }
    return false;
  }

  vector<const term*> context::projection_bindings(const term* t) {
    assert(t->t->arity() > 0);
    auto ys = outer_imitation_binding_args(t);
    vector<const term*> projection_bindings;
    for (auto y : ys) {
      auto inner_args = inner_imitation_binding_args(ys, y);
      auto pb = append_lambdas(ys, apply_args(y, inner_args));
      projection_bindings.push_back(pb);
    }
    return projection_bindings;
  }

  // TODO: Currently this function only ever adds the 1st
  // imitation binding. Will need a more sophisticated search strategy
  // to accomodate all bindings
  bool context::add_projection_binding(stt_res::disagreement_set& s) {
    tp to_sub;
    auto sub_any = false;
    for (auto p : s) {
      auto left_lam_term = split_leading_lambdas(p.first);
      auto right_lam_term = split_leading_lambdas(p.second);
      if (vars_equal(left_lam_term.first, right_lam_term.first)) {
	auto left_head_and_args = split_args(left_lam_term.second);
	auto right_head_and_args = split_args(right_lam_term.second);
	auto left_head = left_head_and_args.first;
	if (left_head->is_var() && left_head->t->arity() > 0) {
	    sub_any = true;
	    auto ps = projection_bindings(left_head);
	    to_sub = tp(left_head, ps[0]);
	}
      }
    }
    if (sub_any) {
      cout << "proj binding is " << endl;
      cout << *(to_sub.first) << " , " << *(to_sub.second) << endl;
      s.insert(to_sub);
      return true;
    }
    return false;
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

  void context::reduce_pair_args(disagreement_set& s) {
    for (auto p : s) {
      auto subpairs = reduce_args(p.first, p.second);
      if (subpairs.size() > 0) {
	cout << "reduce_pair_args" << endl;
	cout << "to erase " << *(p.first) << ", " << *(p.second) << endl;
	cout << "to insert " << endl;
	for (auto p : subpairs) {
	  cout << "-- " << *(p.first) << " / " << *(p.second) << endl;
	}		
	s.erase_pair(p);
	s.add_pairs(subpairs);
	break;
      }
    }
  }

  pair<const var*, tp> context::pair_is_solvable(tp p) {
    auto ts = term_solvable(p.first, p.second);
    if (ts.first != nullptr) {
      return ts;
    }
    return term_solvable(p.second, p.first);
  }

  pair<const var*, tp> context::term_solvable(const term* l, const term* r) {
    auto matched = match_lambdas(l, r);
    auto lam_vars = matched.first;
    auto ft = matched.second.first;
    auto la = split_args(ft);
    if (la.first->is_var()) {
      auto potential = static_cast<const var*>(la.first);
      if (!free_in(potential, r)) {
	if (la.second.size() == lam_vars.size()) {
	  auto all_vars = true;
	  for (int i = 0; i < lam_vars.size(); i++) {
	    auto e = la.second[i];
	    auto v = lam_vars[i];
	    if (*e != *v) {
	      all_vars = false;
	    }
	  }
	  if (all_vars) {
	    return pair<const var*, tp>(potential, tp(l, r));
	  }
	}
      }
    }
    tp dummy;
    return pair<const var*, tp>(nullptr, dummy);
  }

  pair<const var*, tp> context::find_solvable_pair(disagreement_set& s) {
    for (auto p : s) {
      auto res = pair_is_solvable(p);
      if (res.first != nullptr) {
	return res;
      }
    }
    tp to_solve;
    const var* val = nullptr;    
    return pair<const var*, tp>(val, to_solve);
  }

  bool context::solve_vars(disagreement_set& s) {
    auto r = find_solvable_pair(s);
    auto val = r.first;
    auto to_solve = r.second;
    
    if (val == nullptr) {
      return false;
    }

    s.erase_pair(to_solve);
    
    auto new_pair = tp(val, to_solve.second);
    stt_res::sub new_s{new_pair};
    auto action = [this, &new_s](tp p) { return tp(apply_sub(new_s, p.first), apply_sub(new_s, p.second)); };
    s.apply(action);
    s.insert(new_pair);
    return true;
  }

  bool context::unify_dfs(disagreement_set& s, int depth) {
    s.delete_identical_pairs();
    solve_vars(s);
    s.delete_identical_pairs();
    reduce_pair_args(s);
    s.delete_identical_pairs();
    if (s.is_solved()) {
      return true;
    }
    if (depth == 0) {
      return false;
    }
    auto before_delete = s;
    auto current = before_delete;
    auto res = false;
    if (add_imitation_binding(current)) {
      res = unify_dfs(current, depth - 1);
      if (res) {
	s = current;
	return res;
      }
    }
    current = before_delete;
    if (add_projection_binding(current)) {
      res = unify_dfs(current, depth - 1);
      if (res) {
    	s = current;
    	return res;
      }
    }
    return false;
  }

  bool context::unify(disagreement_set& s) {
    int depth = 6;
    return unify_dfs(s, depth);
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
    if (t->is_con()) {
      return t;
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
      auto new_lam = static_cast<const term*>(mk_lam(v, *t_loc));
      t_loc = &new_lam;
    }
    return *t_loc;
  }

}
