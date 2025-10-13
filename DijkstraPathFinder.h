#pragma once

#include "PathFinder.h"

class DijkstraPathFinder : public PathFinder {
  static void find_path_to_others(const DistanceMatrix &node_distances,
                                  distance_t *minimal_paths,
                                  node_t start_node);

public:
  void find_paths(const DistanceMatrix &node_distances,
                  const DistanceMatrix &minimal_paths) const override;
};