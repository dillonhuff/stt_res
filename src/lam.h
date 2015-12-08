#ifndef STT_RES_LAM_H
#define STT_RES_LAM_H

#include <string>

#include "src/term.h"
#include "src/var.h"

using namespace std;

namespace stt_res {
  
  class lam : public term {
  protected:
    var* v;
    term* e;
    
  public:
  lam(var* vp, term* ep) : v(vp), e(ep) {}

    virtual bool is_lam() const override {
      return true;
    }
    
    virtual bool operator==(const term& other) const override {
      if (other.is_lam()) {
	auto o = static_cast<const lam&>(other);
	return *v == *(o.v) && *e == *(o.e);
      } else {
	return false;
      }
    }

  };

}

#endif
