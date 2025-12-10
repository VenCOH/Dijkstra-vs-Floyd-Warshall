#pragma once

#include <limits>
#include <cstdint>

#include "Matrix.h"

typedef uint32_t node_t;
typedef float distance_t;

typedef Matrix<distance_t, 1.0e-5L> DistanceMatrix;

constexpr distance_t DISTANCE_INFINITY = std::numeric_limits<
  distance_t>::infinity();

class PathFinder {
public:
  PathFinder() = default;
  virtual void find_paths(const DistanceMatrix &transition_matrix,
                          DistanceMatrix &paths) const = 0;
  virtual ~PathFinder() = default;
};