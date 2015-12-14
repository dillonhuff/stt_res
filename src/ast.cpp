#include "src/ast.h"

namespace stt_res {
  
  vector<const var*> free_vars(const term* t) {
    
    if (t->is_var()) {
      auto v = static_cast<const var*>(t);
      vector<const var*> fvs{v};
      return fvs;
    }
    
    if (t->is_ap()) {
      auto a = static_cast<const ap*>(t);
      auto lfvs = free_vars(a->l);
      auto rfvs = free_vars(a->r);
      lfvs.insert(lfvs.end(), rfvs.begin(), rfvs.end());
      return lfvs;
    }

    if (t->is_lam()) {
      auto l = static_cast<const lam*>(t);
      auto efvs = free_vars(l->e);
      auto val = *(l->v);
      efvs.erase(remove_if(efvs.begin(), efvs.end(), [val](const var* x){return val == *x;}), efvs.end());
      return efvs;
    }

    if (t->is_con()) {
      vector<const var*> fvs{};
      return fvs;
    }

    assert(false);
  }

  bool free_in(const var* v, const term* t) {
    auto fvs = free_vars(t);
    auto var_eq = [v](const var* other) { return *other == *v; };
    return count_if(fvs.begin(), fvs.end(), var_eq) > 0;
  }

  pair<vector<const var*>, const term*> split_leading_lambdas(const term* t) {
    const term* const* t_loc = &t;
    vector<const var*> vs;
    while (true) {
      auto m = *t_loc;
      if (m->is_lam()) {
	auto m_lam = static_cast<const lam*>(m);
	vs.push_back(m_lam->v);
	t_loc = &(m_lam->e);
      } else {
	return pair<vector<const var*>, const term*>(vs, m);
      }
    }
    assert(false);
  }

  pair<const term*, vector<const term*>> split_args(const term* t) {
    const term* const* t_loc = &t;
    vector<const term*> args;
    while (true) {
      auto m = *t_loc;
      if (!m->is_ap()) {
	reverse(args.begin(), args.end());
	return pair<const term*, vector<const term*>>(m, args);
      }
      auto m_ap = static_cast<const ap*>(m);
      args.push_back(m_ap->r);
      t_loc = &(m_ap->l);
    }
    assert(false);
  }

  bool vars_equal(vector<const var*> l, vector<const var*> r) {
    if (l.size() != r.size()) {
      return false;
    }
    for (int i = 0; i < l.size(); i++) {
      if (*(l[i]) != *(r[i])) {
	return false;
      }
    }
    return true;
  }

  pair<vector<const var*>, pair<const term*, const term*>> match_lambdas(const term* l, const term* r) {
    const term* const* l_loc = &l;
    const term* const* r_loc = &r;
    vector<const var*> vs;
    while ((*l_loc)->is_lam() && (*r_loc)->is_lam()) {
      auto l_m = static_cast<const lam*>(*l_loc);
      auto r_m = static_cast<const lam*>(*r_loc);
      if (*(l_m->v) == *(r_m->v)) {
	vs.push_back(l_m->v);
      } else {
	break;
      }
      l_loc = &(l_m->e);
      r_loc = &(r_m->e);
    }
    return pair<vector<const var*>, pair<const term*, const term*>>(vs, pair<const term*, const term*>(*l_loc, *r_loc));
  }

  ostream& operator<<(ostream& stream, stt_res::sub& other) {
    for (auto p : other) {
      stream << "[ " << *(p.first) << " / " << *(p.second) << " ]" << endl;
    }
    stream << endl;
    return stream;
  }

}
