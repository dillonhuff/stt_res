#ifndef STT_RES_VAR_H
#define STT_RES_VAR_H

#include <iostream>
#include <string>

#include "src/term.h"

using namespace std;

namespace stt_res {
  
  class var : public term {
  public:
    string name;
    
  var(string n) : name(n) {}

    virtual bool is_var() const override {
      return true;
    }

    virtual bool operator==(const term& other) const override {
      if (other.is_var()) {
	const var& o = static_cast<const var&>(other);
	return name == o.name;
      } else {
	return false;
      }
    }

    virtual void print(ostream& stream) const override {
      stream << name;
    }

  };

}

#endif
