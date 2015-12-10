#ifndef STT_RES_TVAR_H
#define STT_RES_TVAR_H

#include <string>

#include "src/type.h"

class tvar : public type {
 public:
  string name;

 tvar(string n) : name(n) {}
  
  virtual bool is_var() const override {
    return true;
  }
};

#endif
