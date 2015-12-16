#ifndef STT_RES_ASSUMPTION_H
#define STT_RES_ASSUMPTION_H

#include <cassert>
#include <vector>

#include "src/proof.h"

using namespace std;

namespace stt_res {
  
  class assumption : public proof {
  public:

    assumption(const term* r) {
      result = r;
    }
    
    virtual bool is_assumption() const {
      return true;
    }

    virtual bool is_resolution() const {
      return false;
    }

    virtual bool operator==(const assumption& other) const {
      assert(false);
    }
    virtual void print(ostream& stream) const override { assert(false); }

  };

}

#endif
