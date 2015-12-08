#ifndef STT_RES_TERM_H
#define STT_RES_TERM_H

#include <cassert>
#include <string>

#include "src/term.h"

using namespace std;

namespace stt_res {
  
  class term {
  public:

    virtual bool isVar() const {
      return false;
    }

    virtual bool operator==(const term& other) const {
      assert(false);
    }

    virtual bool operator!=(const term& other) const {
      return !(*this == other);
    }

  };

}

#endif
