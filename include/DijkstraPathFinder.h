#pragma once

#include "PathFinder.h"

#include <deque>
#include <utility>
#include <vector>

class DijkstraPathFinder : public PathFinder {
protected:
  static void find_path_to_others(const DistanceMatrix &node_distances,
                                  distance_t *minimal_paths, node_t start_node);

  static void
  find_path_to_others_small(const distance_t *__restrict node_distances,
                            distance_t *minimal_paths, node_t start_node,
                            const node_t *__restrict edges, node_t node_count);

public:
  void find_paths(const DistanceMatrix &node_distances,
                  DistanceMatrix &minimal_paths) const override;
};