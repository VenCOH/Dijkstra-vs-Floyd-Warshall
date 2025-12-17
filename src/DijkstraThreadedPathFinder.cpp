#include "../include/DijkstraThreadedPathFinder.h"

#include "omp.h"

#include <thread>
#include <vector>

void DijkstraPathFinderOMP::find_paths(const DistanceMatrix &node_distances,
                                       DistanceMatrix &minimal_paths) const {
  node_t node_count = node_distances.dimension;

#pragma omp parallel for default(none)                                         \
    shared(node_distances, node_count, minimal_paths)
  for (node_t node = 0; node < node_count; node++)
    find_path_to_others(node_distances, minimal_paths[node], node);
}
void DijkstraPathFinderOMPSmall::find_paths(
    const DistanceMatrix &node_distances, DistanceMatrix &minimal_paths) const {
  node_t node_count = node_distances.dimension;
  auto *edges =
      (node_t *)malloc((node_count) * (node_count + 1) * sizeof(node_t));

  for (node_t node = 0; node < node_count; node++) {
    node_t cnt = 0;
    for (node_t node2 = 0; node2 < node_count; node2++) {
      if (node_distances[node][node2] != DISTANCE_INFINITY)
        edges[node*(node_count+1) + ++cnt] = node2;
    }
    edges[node*(node_count+1)] = cnt;
  }

#pragma omp parallel for default(shared) schedule(dynamic)
  for (node_t node = 0; node < node_count; node++) {
    find_path_to_others_small(node_distances[0], minimal_paths[node], node,
                              edges, node_count);
  }

  free(edges);
}