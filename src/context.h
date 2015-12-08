#ifndef STT_RES_CONTEXT_H
#define STT_RES_CONTEXT_H

#include <cassert>
#include <string>

#include "src/ast.h"

using namespace std;

namespace stt_res {
  
  class context {
  public:

    var* mk_var(string name) { return new var(name); }
    ap* mk_ap(term* l, term* r) { return new ap(l, r); }
    lam* mk_lam(var* v, term* e) { return new lam(v, e); }

  };

}

#endif
