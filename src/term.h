#ifndef STT_RES_TERM_H
#define STT_RES_TERM_H

#include <cassert>
#include <string>

#include "src/term.h"

using namespace std;

namespace stt_res {
  
  class term {
  public:

    virtual bool is_lam() const {
      return false;
    }

    virtual bool is_var() const {
      return false;
    }

    virtual bool is_ap() const {
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
