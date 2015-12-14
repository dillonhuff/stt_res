#include "src/disagreement_set.h"

namespace stt_res {
  
  bool disagreement_set::is_solved() {
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

  ostream& operator<<(ostream& stream, disagreement_set& other) {
    for (auto p : other) {
      stream << "< " << *(p.first) << " , " << *(p.second) << " >" << endl;
    }
    stream << endl;
    return stream;
  }
}
