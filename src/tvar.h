#ifndef STT_RES_TVAR_H
#define STT_RES_TVAR_H

#include <string>

#include "src/type.h"

namespace stt_res {
  
  class tvar : public type {
  public:
    string name;

  tvar(string n) : name(n) {}
  
    virtual bool is_var() const override {
      return true;
    }

    virtual bool operator==(const type& other) const override {
      if (other.is_var()) {
	auto o = static_cast<const tvar&>(other);
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
