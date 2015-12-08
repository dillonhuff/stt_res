#ifndef STT_RES_VAR_H
#define STT_RES_VAR_H

#include <string>

using namespace std;

namespace stt_res {
  
  class var : public term {
  protected:
    string name;
    
  public:
  var(string n) : name(n) {}

    virtual bool isVar() const override {
      return true;
    }

    virtual bool operator==(const term& other) const override {
      if (other.isVar()) {
	const var& o = static_cast<const var&>(other);
	return name == o.name;
      } else {
	return false;
      }
    }

  };

}

#endif
