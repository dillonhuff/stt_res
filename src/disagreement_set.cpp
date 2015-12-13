#include "src/disagreement_set.h"

namespace stt_res {
  
  void disagreement_set::delete_duplicates() {
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

  bool disagreement_set::is_solved() {
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
