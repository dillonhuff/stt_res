#ifndef STT_RES_TFUNC_H
#define STT_RES_TFUNC_H

#include <string>

#include "src/type.h"

class tfunc : public type {
 public:
  const type* in;
  const type* out;

 tfunc(const type* in , const type* out) : in(in), out(out) {}
  
  virtual bool is_var() const override {
    return true;
  }
};

#endif
