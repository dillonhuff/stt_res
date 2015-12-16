#include "src/resolution.h"

namespace stt_res {

  proof* try_to_resolve(context& c, const proof* l, const proof* r) {
    auto l_terms = l->result->terms();
    auto r_terms = r->result->terms();
    return nullptr;
  }

  vector<proof*> new_resolutions(context& c, vector<proof*> current) {
    vector<proof*> ps;
    for (auto l : current) {
      for (auto r : current) {
	auto next = try_to_resolve(c, l, r);
	if (next != nullptr) {
	  ps.push_back(next);
	}
      }
    }
    return ps;
  }

  proof* resolve(context& c, vector<proof*> assumptions) {
    while (true) {
      auto new_proofs = new_resolutions(c, assumptions);
      if (new_proofs.size() == 0) {
	return nullptr;
      } else {
	assumptions.insert(assumptions.end(), new_proofs.begin(), new_proofs.end());
      }
    }
    assert(false);
  }
  
}
