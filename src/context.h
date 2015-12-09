#ifndef STT_RES_CONTEXT_H
#define STT_RES_CONTEXT_H

#include <cassert>
#include <string>

#include "src/ast.h"

using namespace std;

namespace stt_res {
  
  class context {
  public:

    const var* mk_var(string name) { return new var(name); }
    const ap* mk_ap(const term* l, const term* r) { return new ap(l, r); }
    const lam* mk_lam(const var* v, const term* e) { return new lam(v, e); }

    const term* sub(const var* target, const term* replacement, const term* t);

  };

}

#endif
