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

  bool is_sentence(const term* t) {
    if (t == empty_clause) {
      return true;
    }
    auto fvs = free_vars(t);
    return fvs.size() == 0;
  }

  bool all_sentences(vector<proof*> assumptions) {
    return count_if(assumptions.begin(), assumptions.end(), [](proof* a) { return !is_sentence(a->result); }) == 0;
  }

  void preprocess(context& c,
		  proof* p,
		  vector<proof*>& results) {
    auto r = p->result;
    
    results.push_back(p);
  }

  void preprocess_all(context& c,
		      vector<proof*>& assumptions,
		      vector<proof*>& results) {
    for (auto a : assumptions) {
      preprocess(c, a, results);
    }
  }

  proof* resolve(context& c, vector<proof*> assumptions) {
    assert(all_sentences(assumptions));
    for (auto a : assumptions) {
      if (a->result == empty_clause) {
	return a;
      }
    }
    vector<proof*> clauses;
    preprocess_all(c, assumptions, clauses);
    while (true) {
      auto new_proofs = new_resolutions(c, clauses);
      if (new_proofs.size() == 0) {
	return nullptr;
      } else {
	for (auto p : new_proofs) {
	  if (is_refutation(p)) {
	    return p;
	  }
	}
	clauses.insert(clauses.end(), new_proofs.begin(), new_proofs.end());
      }
    }
    assert(false);
  }
  
}
