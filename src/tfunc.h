#ifndef STT_RES_TFUNC_H
#define STT_RES_TFUNC_H

#include <string>

#include "src/type.h"

namespace stt_res {
  
  class tfunc : public type {
  public:
    const type* in;
    const type* out;

  tfunc(const type* in , const type* out) : in(in), out(out) {}

    ~tfunc() {}
  
    virtual bool is_func() const override {
      return true;
    }

    virtual bool operator==(const type& other) const override {
      if (other.is_func()) {
	auto other_f = static_cast<const tfunc&>(other);
	return *in == *(other_f.in) && *out == *(other_f.out);
      }
      return false;
    }
  
  };

}
#endif
