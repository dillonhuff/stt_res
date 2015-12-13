#ifndef STT_RES_DISAGREEMENT_SET_H
#define STT_RES_DISAGREEMENT_SET_H

#include "src/ast.h"

namespace stt_res {
  
  class disagreement_set {
  protected:
    stt_res::sub s;
    
  public:

    stt_res::sub extract_sub() {
      return s;
    }
    
    disagreement_set(stt_res::sub sub) {
      for (auto p : sub) {
	push_back(p);
      }
    }

    stt_res::sub::iterator begin() noexcept {
      return s.begin();
    }

    stt_res::sub::iterator end() noexcept {
      return s.end();
    }

    void push_back(tp r) {
      s.push_back(r);
    }

    int size() {
      return s.size();
    }

    tp operator[](int i) {
      return s[i];
    }

    void delete_identical_pairs() {
      s.erase(remove_if(s.begin(), s.end(), [](tp p) {return *(p.first) == *(p.second);}), s.end());
    }

    void add_pairs(stt_res::sub& subpairs) {
      s.insert(s.end(), subpairs.begin(), subpairs.end());
    }

    void erase_pair(stt_res::tp p) {
      s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.first) && *(r.second) == *(p.second); }), s.end());
      s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.second) && *(r.second) == *(p.first); }), s.end());
    }

    void delete_duplicates() {
      stt_res::sub duplicates;
      for (auto p : s) {
	auto num_instances = count_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.first) && *(r.second) == *(p.second); });
	if (num_instances > 1) {
	  duplicates.push_back(p);
	}
      }
      for (auto p : duplicates) {
	erase_pair(p);
      }
      for (auto p : duplicates) {
	push_back(p);
      }
    }    
    
  };
 
}

#endif
