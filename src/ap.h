#ifndef STT_RES_AP_H
#define STT_RES_AP_H

#include <iostream>
#include <string>

#include "src/ast.h"
#include "src/term.h"
#include "src/tfunc.h"

using namespace std;

namespace stt_res {
  
  class ap : public term {
  protected:
    
  public:
    const term* l;
    const term* r;
    
    ap(const term* lp, const term* rp) {
      l = lp;
      r = rp;
      cout << "start ap" << endl;
      cout << "-- " << *lp << "\t\t" << *rp << endl;
      if (lp->t->is_func()) {
      	auto l_t_f = static_cast<const tfunc*>(lp->t);
      	auto l_t_in = l_t_f->in;
      	assert(*l_t_in == *(rp->t));
      	l = lp;
      	r = rp;
      	t = l_t_f->out;
      }
      cout << "end ap" << endl;
    }

    virtual bool is_ap() const override {
      return true;
    }
    
    virtual bool operator==(const term& other) const override {
      if (other.is_ap()) {
	auto o = static_cast<const ap&>(other);
	return (*l == *(o.l)) && (*r == *(o.r));
      } else {
	return false;
      }
    }

    virtual void print(ostream& stream) const override {
      stream << "(";
      l->print(stream);
      stream << " ";
      r->print(stream);
      stream << ")";
    }

  };

}

#endif
