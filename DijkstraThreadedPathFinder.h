#pragma once

#include "DijkstraPathFinder.h"

class DijkstraThreadedPathFinder final : public DijkstraPathFinder {
public:
  void find_paths(const DistanceMatrix &node_distances,
                  const DistanceMatrix &minimal_paths) const override;
};

class DijkstraThreadedPathFinderSmart final : public DijkstraPathFinder {
  const size_t m_Thread_count;

  void find_paths_to_others(const DistanceMatrix &node_distances,
                            const DistanceMatrix &minimal_paths,
                            node_t start_node, node_t end_node);

public:
  explicit DijkstraThreadedPathFinderSmart(const size_t thread_count) :
    DijkstraPathFinder(),
    m_Thread_count(thread_count) {
  }

  void find_paths(const DistanceMatrix &node_distances,
                  const DistanceMatrix &minimal_paths) const override;
};