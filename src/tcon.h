#ifndef STT_RES_TCON_H
#define STT_RES_TCON_H

#include <string>

#include "src/type.h"

namespace stt_res {
  
  class tcon : public type {
  public:
    string name;

  tcon(string n) : name(n) {}
  
    virtual bool is_con() const override {
      return true;
    }

    virtual bool operator==(const type& other) const override {
      if (other.is_con()) {
	auto o = static_cast<const tcon&>(other);
	return name == o.name;
      }
      return false;
    }

    virtual void print(ostream& stream) const override {
      stream << name;
    }

    virtual vector<const type*> arg_types() const override {
      vector<const type*> v{};
      return v;
    }

    virtual int arity() const override {
      return 0;
    }
  };

}
#endif
