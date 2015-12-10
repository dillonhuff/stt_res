#include "src/term.h"

namespace stt_res {

  std::ostream& operator<<(std::ostream& stream, const term& other) {
    other.print(stream);
    return stream;
  }
  
}
