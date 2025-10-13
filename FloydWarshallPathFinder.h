#pragma once
#include "PathFinder.h"

class FloydWarshallPathFinder final : public PathFinder {
public:
  void find_paths(const TMatrix &transition_matrix,
                  const TMatrix &paths) const override;
};