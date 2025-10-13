#include "DijkstraPathFinder.h"

#include <algorithm>
#include <utility>
#include <deque>
#include <queue>
#include <vector>

void DijkstraPathFinder::find_path_to_others(
    const DistanceMatrix &node_distances, distance_t *const minimal_paths,
    const node_t start_node) {
  const size_t node_count = node_distances.dimension;
  std::fill_n(minimal_paths, node_count, DISTANCE_INFINITY);
  minimal_paths[start_node] = 0;
  std::priority_queue<std::pair<distance_t, node_t>, std::deque<std::pair<
                        distance_t, node_t>>, std::greater<>> open_nodes;
  open_nodes.emplace(0, start_node);
  std::vector<bool> closed(node_count);
  while (!open_nodes.empty()) {
    const distance_t current_distance = open_nodes.top().first;
    const node_t current_node = open_nodes.top().second;
    open_nodes.pop();
    if (!closed[current_node]) {
      closed[current_node] = true;
      for (size_t node = 0; node < node_count; node++)
        if (!closed[node])
          if (const distance_t new_distance =
                current_distance + node_distances[current_node][node];
            node_distances[current_node][node] != DISTANCE_INFINITY &&
            new_distance < minimal_paths[node]) {
            minimal_paths[node] = new_distance;
            open_nodes.emplace(new_distance, node);
          }
    }
  }
}

void DijkstraPathFinder::find_paths(const DistanceMatrix &node_distances,
                                    const DistanceMatrix &minimal_paths) const {
  const node_t node_count = node_distances.dimension;

  for (node_t node = 0; node < node_count; node++)
    find_path_to_others(node_distances, minimal_paths[node], node);
}