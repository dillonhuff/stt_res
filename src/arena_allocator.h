#ifndef STT_RES_ARENA_ALLOCATOR_H
#define STT_RES_ARENA_ALLOCATOR_H

#include <vector>

using namespace std;

#define DEFAULT_ARENA_SIZE 1000000

namespace stt_res {
  
  class arena_allocator {
  protected:
    char* start;
    char* current;
    size_t size;
    size_t space_left;
    
  public:
    arena_allocator() {
      size = DEFAULT_ARENA_SIZE;
      space_left = size;
      start = (char*) malloc(size);
      current = start;
    }

    ~arena_allocator() {
      delete start;
    }

    void* allocate(size_t s) {
      space_left = space_left - s;
      assert(space_left > 0);
      auto to_alloc = current;
      current += s;
      return to_alloc;
    }
  };
 
}

#endif
