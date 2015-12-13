#include "src/disagreement_set.h"

namespace stt_res {
  
  bool disagreement_set::is_solved() {
      // for (int i = 0; i < s.size(); i++) {
      // 	auto pair_solved = true;
      // 	auto l = s[i].first;
      // 	auto r = s[i].second;
      // 	if (l->is_var() &&
      // 	    !free_in(static_cast<const var*>(l), r)) {
      // 	  auto vr = static_cast<const var*>(l);
      // 	  auto doesnt_appear_elsewhere = true;
      // 	  for (int j = 0; j < s.size(); j++) {
      // 	    if (i != j) {
      // 	      if (free_in(vr, s[j].first) || free_in(vr, s[j].second)) {
      // 		doesnt_appear_elsewhere = false;
      // 	      }
      // 	    }
      // 	  }
      // 	  if (!doesnt_appear_elsewhere) {
      // 	    pair_solved = false;
      // 	  }
      // 	} else {
      // 	  return false;
      // 	}
      // 	if (!pair_solved) {
      // 	  return false;
      // 	}
      // }
      // return true;
      auto num_solved_pairs = count_if(s.begin(), s.end(), [this](const tp& p) {return pair_is_solved(p.first, p.second);});
      auto num_pairs = s.size();
      return num_solved_pairs == num_pairs;
    }


  bool disagreement_set::pair_is_solved(const term* l, const term* r) {
      if (l->is_var()) {
	auto vr = static_cast<const var*>(l);
	return !free_in(vr, r);
      }
      return false;
    }
  
}
