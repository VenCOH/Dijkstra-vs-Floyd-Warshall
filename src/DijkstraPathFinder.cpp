#include "../include/DijkstraPathFinder.h"

#include <algorithm>
#include <deque>
#include <queue>
#include <utility>
#include <vector>

void DijkstraPathFinder::find_path_to_others(
    const DistanceMatrix &node_distances, distance_t *const minimal_paths,
    const node_t start_node) {
  const size_t node_count = node_distances.dimension;
  std::fill_n(minimal_paths, node_count, DISTANCE_INFINITY);
  minimal_paths[start_node] = 0;
  std::priority_queue<std::pair<distance_t, node_t>,
                      std::deque<std::pair<distance_t, node_t>>, std::greater<>>
      open_nodes;
  open_nodes.emplace(0, start_node);
  std::vector<bool> closed(node_count);
  while (!open_nodes.empty()) {
    const distance_t current_distance = open_nodes.top().first;
    const node_t current_node = open_nodes.top().second;
    open_nodes.pop();
    if (!closed[current_node]) {
      closed[current_node] = true;
      size_t nodeo = 0;

      for (; nodeo + 4 <= node_count; nodeo += 4) {
        const distance_t new_distance0 =
            current_distance + node_distances[current_node][nodeo];
        const distance_t new_distance1 =
            current_distance + node_distances[current_node][nodeo + 1];
        const distance_t new_distance2 =
            current_distance + node_distances[current_node][nodeo + 2];
        const distance_t new_distance3 =
            current_distance + node_distances[current_node][nodeo + 3];

        const bool b0 =
            !closed[nodeo] &&
            node_distances[current_node][nodeo] != DISTANCE_INFINITY &&
            new_distance0 < minimal_paths[nodeo];
        const bool b1 =
            !closed[nodeo + 1] &&
            node_distances[current_node][nodeo + 1] != DISTANCE_INFINITY &&
            new_distance1 < minimal_paths[nodeo + 1];
        const bool b2 =
            !closed[nodeo + 2] &&
            node_distances[current_node][nodeo + 2] != DISTANCE_INFINITY &&
            new_distance2 < minimal_paths[nodeo + 2];
        const bool b3 =
            !closed[nodeo + 3] &&
            node_distances[current_node][nodeo + 3] != DISTANCE_INFINITY &&
            new_distance3 < minimal_paths[nodeo + 3];

        if (b0) {
          minimal_paths[nodeo] = new_distance0;
          open_nodes.emplace(new_distance0, nodeo);
        }

        if (b1) {
          minimal_paths[nodeo + 1] = new_distance1;
          open_nodes.emplace(new_distance1, nodeo + 1);
        }

        if (b2) {
          minimal_paths[nodeo + 2] = new_distance2;
          open_nodes.emplace(new_distance2, nodeo + 2);
        }

        if (b3) {
          minimal_paths[nodeo + 3] = new_distance3;
          open_nodes.emplace(new_distance3, nodeo + 3);
        }
      }

      for (size_t node = nodeo; node < node_count; node++)
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

void DijkstraPathFinder::find_path_to_others_small(
    const distance_t *const __restrict node_distances,
    distance_t *const minimal_paths, const node_t start_node,
    const node_t *const __restrict edges, const node_t node_count) {
  std::fill_n(minimal_paths, node_count, DISTANCE_INFINITY);
  minimal_paths[start_node] = 0;
  std::priority_queue<std::pair<distance_t, node_t>,
                      std::deque<std::pair<distance_t, node_t>>,
                      std::greater<>>
      open_nodes;
  open_nodes.emplace(0, start_node);
  std::vector<bool> closed(node_count);
  while (!open_nodes.empty()) {
    const distance_t current_distance = open_nodes.top().first;
    const node_t current_node = open_nodes.top().second;
    open_nodes.pop();
    if (!closed[current_node]) {
      closed[current_node] = true;
      node_t count = edges[current_node * (node_count + 1)];

      node_t io = 1;
      for (; io + 3 <= count; io += 4) {
        const node_t node0 = edges[current_node * (node_count + 1) + io];
        const node_t node1 = edges[current_node * (node_count + 1) + io + 1];
        const node_t node2 = edges[current_node * (node_count + 1) + io + 2];
        const node_t node3 = edges[current_node * (node_count + 1) + io + 3];

        const distance_t new_distance0 =
            current_distance +
            node_distances[current_node * node_count + node0];
        const distance_t new_distance1 =
            current_distance +
            node_distances[current_node * node_count + node1];
        const distance_t new_distance2 =
            current_distance +
            node_distances[current_node * node_count + node2];
        const distance_t new_distance3 =
            current_distance +
            node_distances[current_node * node_count + node3];

        const bool b0 = !closed[node0] &&
                        node_distances[current_node * node_count + node0] !=
                            DISTANCE_INFINITY &&
                        new_distance0 < minimal_paths[node0];
        const bool b1 = !closed[node1] &&
                        node_distances[current_node * node_count + node1] !=
                            DISTANCE_INFINITY &&
                        new_distance1 < minimal_paths[node1];
        const bool b2 = !closed[node2] &&
                        node_distances[current_node * node_count + node2] !=
                            DISTANCE_INFINITY &&
                        new_distance2 < minimal_paths[node2];
        const bool b3 = !closed[node3] &&
                        node_distances[current_node * node_count + node3] !=
                            DISTANCE_INFINITY &&
                        new_distance3 < minimal_paths[node3];

        if (b0) {
          minimal_paths[node0] = new_distance0;
          open_nodes.emplace(new_distance0, node0);
        }

        if (b1) {
          minimal_paths[node1] = new_distance1;
          open_nodes.emplace(new_distance1, node1);
        }

        if (b2) {
          minimal_paths[node2] = new_distance2;
          open_nodes.emplace(new_distance2, node2);
        }

        if (b3) {
          minimal_paths[node3] = new_distance3;
          open_nodes.emplace(new_distance3, node3);
        }
      }

      for (node_t i = io; i <= count; i++) {
        const node_t node = edges[current_node * (node_count + 1) + i];
        if (!closed[node])
          if (const distance_t new_distance =
                  current_distance +
                  node_distances[current_node * node_count + node];
              node_distances[current_node * node_count + node] !=
                  DISTANCE_INFINITY &&
              new_distance < minimal_paths[node]) {
            minimal_paths[node] = new_distance;
            open_nodes.emplace(new_distance, node);
          }
      }
    }
  }
}

void DijkstraPathFinder::find_paths(const DistanceMatrix &node_distances,
                                    DistanceMatrix &minimal_paths) const {
  const node_t node_count = node_distances.dimension;

  for (node_t node = 0; node < node_count; node++)
    find_path_to_others(node_distances, minimal_paths[node], node);
}