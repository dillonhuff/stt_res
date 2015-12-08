#ifndef STT_RES_AP_H
#define STT_RES_AP_H

#include <string>

#include "src/term.h"

using namespace std;

namespace stt_res {
  
  class ap {
  protected:
    term* l;
    term* r;
    
  public:
  ap(term* lp, term* rp) : l(lp), r(rp) {}

    bool operator==(const ap& other) const {
      return (*l == *(other.l)) && (*r == *(other.r));
    }

  };

}

#endif
