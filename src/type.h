#ifndef STT_RES_TYPE_H
#define STT_RES_TYPE_H

namespace stt_res {
  
  class type {
  public:
    virtual bool is_func() const {
      return false;
    }

    virtual bool is_var() const {
      return false;
    }

    virtual bool operator==(const type& other) const {
      assert(false);
    }

    virtual bool operator!=(const type& other) const {
      return !(*this == other);
    }
  
    virtual int arity() const { assert(false); }
  
  };
 
}

#endif
