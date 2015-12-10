#include <iostream>
#include <cassert>

#include "src/type.h"

using namespace std;

namespace stt_res {
  
  std::ostream& operator<<(std::ostream& stream, const type& other) {
    other.print(stream);
    return stream;
  }
  
}
