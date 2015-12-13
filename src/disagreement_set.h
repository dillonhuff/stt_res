#ifndef STT_RES_DISAGREEMENT_SET_H
#define STT_RES_DISAGREEMENT_SET_H

#include "src/ast.h"

namespace stt_res {

  class disagreement_set {
  protected:
    dset s;
    
  public:
    sub extract_sub() {
      assert(is_solved());
      sub new_sub;
      return new_sub;
    }

    disagreement_set(sub sub) {
      for (auto p : sub) {
	insert(p);
      }
    }

    dset::iterator begin() noexcept { return s.begin(); }
    dset::iterator end() noexcept { return s.end(); }
    void insert(tp r) { s.insert(r); }
    int size() { return s.size(); }
    //    tp operator[](int i) { return s[i]; }

    void delete_identical_pairs() {
      //      s.erase(remove_if(s.begin(), s.end(), [](tp p) {return *(p.first) == *(p.second);}), s.end());
    }

    void add_pairs(sub& subpairs) {
      s.insert(subpairs.begin(), subpairs.end());
    }

    void erase_pair(stt_res::tp p) {
      //      s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.first) && *(r.second) == *(p.second); }), s.end());
      //      s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.second) && *(r.second) == *(p.first); }), s.end());
    }

    //    void delete_duplicates();

    bool pair_is_solved(const term* l, const term* r);
    bool is_solved();

  };
 
}

#endif
