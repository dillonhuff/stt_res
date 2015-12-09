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

    assert(false);
  }

  ostream& operator<<(ostream& stream, const term& other) {
    other.print(stream);
    return stream;
  }

}
