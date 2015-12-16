#ifndef STT_RES_RESOLUTION_H
#define STT_RES_RESOLUTION_H

#include "src/context.h"
#include "src/proof.h"

namespace stt_res {

  proof* resolve(context& c, vector<proof*> assumptions);
  
}

#endif
