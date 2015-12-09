#include "src/ast.h"

namespace stt_res {
  
  ostream& operator<<(ostream& stream, const term& other) {
    if (other.is_var()) {
      auto v = static_cast<const var&>(other);
      stream << v;
      return stream;
    }
    if (other.is_lam()) {
      auto l = static_cast<const lam&>(other);
      stream << l;
      return stream;
    }
    if (other.is_ap()) {
      auto a = static_cast<const ap&>(other);
      stream << a;
      return stream;
    }
    assert(false);
  }

  ostream& operator<<(ostream& stream, const lam& other) {
    stream << "(\\" << *(other.v) << ". " << *(other.e) << ")";
    return stream;
  }

  ostream& operator<<(ostream& stream, const ap& other) {
    stream << "(" << *(other.l) << " " << *(other.r) << ")";
    return stream;
  }


}
