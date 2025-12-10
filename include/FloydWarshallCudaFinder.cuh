#pragma once

#include "PathFinder.h"

class FloydWarshallCudaFinder final : public PathFinder {
public:
  void find_paths(const DistanceMatrix &transition_matrix,
                  DistanceMatrix &paths) const override;
};