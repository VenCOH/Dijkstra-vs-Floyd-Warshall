#include "DijkstraThreadedPathFinder.h"

#include <thread>
#include <vector>

void DijkstraThreadedPathFinder::find_paths(
    const DistanceMatrix &node_distances,
    const DistanceMatrix &minimal_paths) const {
  const node_t node_count = node_distances.dimension;

  std::vector<std::thread> threads;
  threads.reserve(node_count);
  for (node_t node = 0; node < node_count; node++)
    threads.emplace_back(DijkstraPathFinder::find_path_to_others,
                         node_distances, minimal_paths[node], node);

  for (auto &thread : threads)
    thread.join();
}

void DijkstraThreadedPathFinderSmart::find_paths_to_others(
    const DistanceMatrix &node_distances, const DistanceMatrix &minimal_paths,
    node_t start_node, node_t end_node) {

}

void DijkstraThreadedPathFinderSmart::find_paths(
    const DistanceMatrix &node_distances,
    const DistanceMatrix &minimal_paths) const {
  // TODO
  const node_t node_count = node_distances.dimension;

  std::vector<std::thread> threads;
  threads.reserve(m_Thread_count);
  size_t work_per_thread = node_count / m_Thread_count;
  if (work_per_thread * m_Thread_count < node_count)
    work_per_thread++;
  for (size_t i = 0; i < m_Thread_count; i++) {
    threads.emplace_back(find_paths_to_others,
                         node_distances, minimal_paths[node], node);
  }

  for (auto &thread : threads)
    thread.join();
}