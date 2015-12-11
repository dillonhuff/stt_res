#ifndef STT_RES_TFUNC_H
#define STT_RES_TFUNC_H

#include <iostream>
#include <string>

#include "src/type.h"

namespace stt_res {
  
  class tfunc : public type {
  public:
    const type* in;
    const type* out;

  tfunc(const type* in , const type* out) : in(in), out(out) {}

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

    virtual void print(ostream& stream) const override {
      stream << "(";
      in->print(stream);
      stream << " -> ";
      out->print(stream);
      stream << ")";
    }

    virtual vector<const type*> arg_types() const override {
      auto t = out->arg_types();
      t.push_back(in);
      return t;
    }
  
  };

}
#endif
