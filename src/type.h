#ifndef STT_RES_TYPE_H
#define STT_RES_TYPE_H

class type {

  virtual bool is_func() const {
    return false;
  }

  virtual bool is_var() const {
    return false;
  }
  
  virtual int arity() const { assert(false); }
  
};

#endif
