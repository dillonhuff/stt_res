#include "src/resolution.h"

namespace stt_res {

  bool resolve_sub(context& c, const term* l, const term* r, sub res) {
    const term* m = nullptr;
    const term* n = nullptr;
    if (is_neg(l) && !is_neg(r)) {
      m = destruct_neg(l);
      n = r;
    }
    if (!is_neg(l) && is_neg(r)) {
      m = l;
      n = destruct_neg(r);
    }
    if (m != nullptr && n != nullptr) {
      sub s{tp(m, n)};
      disagreement_set d_set(s);
      auto unify_succeeded = c.unify(d_set);
      if (unify_succeeded) {
	res = d_set.extract_sub();
	return true;
      }
    }
    return false;
  }

  proof* resolved_clause(context& c,
			 stt_res::sub& theta,
			 const term* l_t,
			 const term* r_t,
			 proof* l,
			 proof* r) {
    cout << *l_t << endl;
    cout << *r_t << endl;
    cout << theta << endl;
    auto l_terms = l->result->terms();
    auto r_terms = r->result->terms();
    vector<const term*> res_clause_terms;
    for (auto l_v : l_terms) {
      if (*l_v != *l_t) {
	auto new_l_v = c.apply_sub(theta, l_v);
	res_clause_terms.push_back(new_l_v);
      }
    }
    for (auto r_v : r_terms) {
      if (*r_v != *r_t) {
	auto new_r_v = c.apply_sub(theta, r_v);
	res_clause_terms.push_back(new_r_v);	
      }
    }
    if (res_clause_terms.size() > 0) {
      auto res_clause = c.mk_clause(res_clause_terms);
      return c.mk_res(theta, l, r, res_clause);
    }
    return c.mk_res(theta, l, r, nullptr);
  }

  proof* try_to_resolve(context& c, proof* l, proof* r) {
    auto l_terms = l->result->terms();
    auto r_terms = r->result->terms();
    for (auto l_t : l_terms) {
      for (auto r_t : r_terms) {
	sub theta;
	auto sub_succeeded = resolve_sub(c, l_t, r_t, theta);
	if (sub_succeeded) {
	  cout << "solved unification" << endl;
	  return resolved_clause(c, theta, l_t, r_t, l, r);
	}
      }
    }
    return nullptr;
  }

  vector<proof*> new_resolutions(context& c, vector<proof*> current) {
    vector<proof*> ps;
    for (auto l : current) {
      for (auto r : current) {
	auto next = try_to_resolve(c, l, r);
	if (next != nullptr) {
	  ps.push_back(next);
	}
      }
    }
    return ps;
  }

  bool is_refutation(const proof* p) {
    return p->result == nullptr;
  }

  proof* resolve(context& c, vector<proof*> assumptions) {
    while (true) {
      auto new_proofs = new_resolutions(c, assumptions);
      if (new_proofs.size() == 0) {
	cout << "no new proofs" << endl;
	return nullptr;
      } else {
	cout << "new proofs" << endl;
	for (auto p : new_proofs) {
	  if (is_refutation(p)) {
	    return p;
	  }
	}
	assumptions.insert(assumptions.end(), new_proofs.begin(), new_proofs.end());
      }
    }
    assert(false);
  }
  
}
