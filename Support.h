#pragma once

#include <new>
#include <cstdlib>

inline void *safe_alloc(const size_t element_count, const size_t element_size,
                        const bool zero = false) {
  void *ptr;
  if (zero)
    ptr = calloc(element_count, element_size);
  else
    ptr = malloc(element_count * element_size);

  if (ptr == nullptr)
    throw std::bad_alloc();

  return ptr;
}

inline void safe_free(void *ptr) {
  free(ptr);
}
