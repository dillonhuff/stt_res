#ifndef STT_RES_PROOF_H
#define STT_RES_PROOF_H

#include <cassert>
#include <vector>

#include "src/ast.h"

using namespace std;

namespace stt_res {
  
  class proof {
  public:
    const term* result;
    
    virtual bool is_assumption() const {
      return false;
    }

    virtual bool is_resolution() const {
      return false;
    }

    virtual bool operator==(const proof& other) const {
      assert(false);
    }

    virtual bool operator!=(const proof& other) const {
      return !(*this == other);
    }
  
    virtual void print(ostream& stream) const { assert(false); }

  };

  ostream& operator<<(ostream& stream, const proof& other);
 
}

#endif
