#pragma once

#include "../include/DijkstraPathFinder.h"

class DijkstraPathFinderOMP final : public DijkstraPathFinder {
  void find_paths(const DistanceMatrix &node_distances,
                  DistanceMatrix &minimal_paths) const override;
};

class DijkstraPathFinderOMPSmall final : public DijkstraPathFinder {
  void find_paths(const DistanceMatrix &node_distances,
                  DistanceMatrix &minimal_paths) const override;
};