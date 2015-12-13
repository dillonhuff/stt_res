#ifndef STT_RES_DISAGREEMENT_SET_H
#define STT_RES_DISAGREEMENT_SET_H

#include <functional>

#include "src/ast.h"

using namespace std;

namespace stt_res {

  class disagreement_set {
  protected:
    dset s;
    
  public:
    sub extract_sub() {
      assert(is_solved());
      cout << "is solved" << endl;
      sub new_sub;
      for (auto p : s) {
	cout << *(p.first) << " , " << *(p.second) << endl;
	new_sub.push_back(p);
      }
      return new_sub;
    }

    disagreement_set() {}

    disagreement_set(sub sub) {
      for (auto p : sub) {
	insert(p);
      }
    }

    dset::iterator begin() noexcept { return s.begin(); }
    dset::iterator end() noexcept { return s.end(); }
    void insert(tp r) { s.insert(r); }
    int size() { return s.size(); }

    void delete_identical_pairs() {
      dset new_set;
      copy_if(s.begin(), s.end(), inserter(new_set, new_set.end()), [](const tp& p) {return !(*(p.first) == *(p.second));});
      s = new_set;
    }

    void add_pairs(sub& subpairs) {
      cout << "to insert " << endl;
      for (auto p : subpairs) {
	cout << "-- " << *(p.first) << " / " << *(p.second) << endl;
      }
      for (auto p : subpairs) {
	if (s.count(p) != 0) {
	  cout << "ALREADY IN s" << endl;
	  cout << "-- " << *(p.first) << " / " << *(p.second) << endl;
	}
      }
      cout << "s before add " << endl;
      for (auto p : s) {
	cout << "-- " << *(p.first) << " / " << *(p.second) << endl;
      }		      
      s.insert(subpairs.begin(), subpairs.end());
      cout << "s after add " << endl;
      for (auto p : s) {
	cout << "-- " << *(p.first) << " / " << *(p.second) << endl;
      }		            
    }

    void erase_pair(stt_res::tp p) {
      dset new_set;
      copy_if(s.begin(), s.end(), inserter(new_set, new_set.end()), [p](tp r) { return not (*(r.first) == *(p.first) && *(r.second) == *(p.second)); });
      s = new_set;
      //      s.erase(remove_if(s.begin(), s.end(), 
      //      s.erase(remove_if(s.begin(), s.end(), [p](tp r) { return *(r.first) == *(p.second) && *(r.second) == *(p.first); }), s.end());
    }

    bool pair_is_solved(const term* l, const term* r);
    bool is_solved();

  };
 
}

#endif
