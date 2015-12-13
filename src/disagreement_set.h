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

    bool pair_is_solved(disagreement_set& s, const term* l, const term* r) {
      if (l->is_var()) {
	auto vr = static_cast<const var*>(l);
	return !free_in(vr, r);
      }
      return false;
    }

    bool is_solved() {
      for (int i = 0; i < s.size(); i++) {
	auto pair_solved = true;
	auto l = s[i].first;
	auto r = s[i].second;
	if (l->is_var() &&
	    !free_in(static_cast<const var*>(l), r)) {
	  auto vr = static_cast<const var*>(l);
	  auto doesnt_appear_elsewhere = true;
	  for (int j = 0; j < s.size(); j++) {
	    if (i != j) {
	      if (free_in(vr, s[j].first) || free_in(vr, s[j].second)) {
		doesnt_appear_elsewhere = false;
	      }
	    }
	  }
	  if (!doesnt_appear_elsewhere) {
	    pair_solved = false;
	  }
	} else {
	  return false;
	}
	if (!pair_solved) {
	  return false;
	}
      }
      return true;
      // auto num_solved_pairs = count_if(s.begin(), s.end(), [this, &s](tp p) {return pair_is_solved(s, p.first, p.second);});
      // auto num_pairs = s.size();
      // return num_solved_pairs == num_pairs;
    }
    
    
  };
 
}

#endif
