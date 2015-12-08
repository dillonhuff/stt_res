#ifndef STT_RES_VAR_H
#define STT_RES_VAR_H

#include <string>

using namespace std;

class var {
 public:
  var(string n) {}

  bool operator==(const var& other) const {
    return false;
  }
};

#endif
