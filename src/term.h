#ifndef STT_RES_TERM_H
#define STT_RES_TERM_H

#include <cassert>
#include <string>
#include <vector>

#include "src/type.h"

using namespace std;

namespace stt_res {
  
  class term {
  public:

    const type* t;

    ~term() {}

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

    virtual void print(ostream& other) const {
      assert(false);
    }

  };

}

#endif
