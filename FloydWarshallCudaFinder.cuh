#pragma once

#include "PathFinder.h"

class FloydWarshallCudaFinder final : public PathFinder {
public:
  void find_paths(const DistanceMatrix &transition_matrix,
                  const DistanceMatrix &paths) const override;
};