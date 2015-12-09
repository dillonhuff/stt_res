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

  res_code context::unify(stt_res::sub& s) {
    while (true) {
      if (system_is_solved(s)) {
	return UNIFY_SUCCEEDED;
      }
      delete_identical_pairs(s);
    }
  }
}
