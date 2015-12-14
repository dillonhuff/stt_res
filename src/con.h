#ifndef STT_RES_CON_H
#define STT_RES_CON_H

#include <iostream>
#include <string>

#include "src/term.h"

using namespace std;

namespace stt_res {
  
  class con : public term {
  public:
    string name;

    con(string n, const type* tp) {
      name = n;
      t = tp;
    }

    virtual bool is_con() const override {
      return true;
    }

    virtual bool operator==(const term& other) const override {
      if (other.is_con()) {
	const con& o = static_cast<const con&>(other);
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
