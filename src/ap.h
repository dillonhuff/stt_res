#ifndef STT_RES_AP_H
#define STT_RES_AP_H

#include <string>

#include "src/term.h"

using namespace std;

namespace stt_res {
  
  class ap : public term {
  protected:
    
  public:
    const term* l;
    const term* r;
    
  ap(const term* lp, const term* rp) : l(lp), r(rp) {}

    virtual bool is_ap() const override {
      return true;
    }
    
    virtual bool operator==(const term& other) const override {
      if (other.is_ap()) {
	auto o = static_cast<const ap&>(other);
	return (*l == *(o.l)) && (*r == *(o.r));
      } else {
	return false;
      }
    }

  };

}

#endif
