#ifndef STT_RES_RES_H
#define STT_RES_RES_H

#include "src/proof.h"

using namespace std;

namespace stt_res {
  
  class res : public proof {
  public:

    sub s;
    proof* l;
    proof* r;

    res(sub& s_a, proof* l_a, proof* r_a, const term* res_a) {
      s = s_a;
      l = l_a;
      r = r_a;
      result = res_a;
    }
    
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

}

#endif
