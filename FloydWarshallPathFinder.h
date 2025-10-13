#pragma once
#include "PathFinder.h"

class FloydWarshallPathFinder final : public PathFinder {
public:
  void find_paths(const DistanceMatrix &transition_matrix,
                  const DistanceMatrix &paths) const override;
};