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

    // TODO: Add type checking
    ap(const term* lp, const term* rp, const type* tp) {
      l = lp;
      r = rp;
      t = tp;
    }
    
    ap(const term* lp, const term* rp) {
      l = lp;
      r = rp;
      if (lp->t->is_func()) {
      	auto l_t_f = static_cast<const tfunc*>(lp->t);
      	auto l_t_in = l_t_f->in;
	/* cout << "-- lp: " << *lp << endl; */
	/* cout << "-- rp: " << *rp << endl; */
	/* cout << "-- lp type: " << *(lp->t) << endl; */
	/* cout << "-- rp type: " << *(rp->t) << endl;	 */
      	assert(*l_t_in == *(rp->t));
      	t = l_t_f->out;
      } else {
	cout << "-- lp: " << *lp << endl;
	cout << "-- rp: " << *rp << endl;
	cout << "-- lp type: " << *(lp->t) << endl;
	cout << "-- rp type: " << *(rp->t) << endl;		
	assert(false);
      }
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
