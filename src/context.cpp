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
}
