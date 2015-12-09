#ifndef STT_RES_AST_H
#define STT_RES_AST_H

#include <vector>

#include "src/ap.h"
#include "src/lam.h"
#include "src/var.h"

namespace stt_res {
  
  vector<const var*> free_vars(const term* t);

  ostream& operator<<(ostream& stream, const term& other);

}
#endif
