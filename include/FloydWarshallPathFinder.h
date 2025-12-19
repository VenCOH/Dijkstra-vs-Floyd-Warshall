#pragma once
#include "PathFinder.h"

class FloydWarshallPathFinder final : public PathFinder {
public:
  void find_paths(const DistanceMatrix &transition_matrix,
                  DistanceMatrix &paths) const override;
};

class BlockedFloydWarshallPathFinderOMP final : public PathFinder {
public:
  void find_paths(const DistanceMatrix &transition_matrix,
                  DistanceMatrix &paths) const override;
};

class BlockedFloydWarshallPathFinder final : public PathFinder {
public:
  void find_paths(const DistanceMatrix &transition_matrix,
                  DistanceMatrix &paths) const override;
};