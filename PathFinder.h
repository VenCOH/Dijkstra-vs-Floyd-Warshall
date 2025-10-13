#pragma once

#include "Matrix.h"

typedef Matrix<uint32_t> TMatrix;

constexpr uint32_t T_INFINITY = UINT32_MAX;

class PathFinder {
public:
  PathFinder() = default;
  virtual void find_paths(const TMatrix &transition_matrix,
                          const TMatrix &paths) const = 0;
  virtual ~PathFinder() = default;
};