#include "../include/DijkstraPathFinder.h"
#include "../include/DijkstraThreadedPathFinder.h"
#include "../include/Matrix.h"

#include <random>
#include <chrono>
#include <iostream>

#include "../include/FloydWarshallPathFinder.h"
#include "../include/PathFinder.h"

void run_solver_timed(const std::string &finder_name,
                      const PathFinder &path_finder,
                      const DistanceMatrix &transition_matrix,
                      DistanceMatrix &paths) {
  std::cout << "Running path finder " << finder_name << std::endl;

  const std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();
  path_finder.find_paths(transition_matrix, paths);
  const std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();

  std::cout << "Execution time: " << std::chrono::duration_cast<
        std::chrono::milliseconds>(end_time - start_time).count() << "[ms]" <<
      std::endl;
}

void random_fill_matrix(const DistanceMatrix &matrix, const node_t dimension,
                        const distance_t number_limit,
                        const double edge_exists_chance) {
  const std::chrono::steady_clock::time_point start_time =
        std::chrono::steady_clock::now();

  std::random_device random_source;
  std::mt19937 generator(random_source());
  static constexpr auto max_random_number =
      static_cast<double>(generator.max());
  for (size_t row = 0; row < dimension; row++)
    for (size_t col = 0; col < dimension; col++) {
      if (row == col)
        matrix[row][col] = 0;
      else {
        double chance = generator() / max_random_number;
        if (chance < edge_exists_chance)
          matrix[row][col] = static_cast<distance_t>(
                               generator() % static_cast<
                                 std::mt19937::result_type>(
                                 number_limit)) / static_cast<distance_t>(
                               100.0);
        else
          matrix[row][col] = DISTANCE_INFINITY;
      }
    }

  const std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();

  std::cout << "Generation took: " << std::chrono::duration_cast<
        std::chrono::milliseconds>(end_time - start_time).count() << "[ms]" <<
      std::endl;
}

#ifndef NODE_COUNT
#define NODE_COUNT 10
#endif

#ifndef NUMBER_LIMIT
#define NUMBER_LIMIT 4000
#endif

#ifndef EDGE_EXISTS_CHANCE
#define EDGE_EXISTS_CHANCE 0.5
#endif

int main() {
  const DistanceMatrix matrix(NODE_COUNT, true);

  random_fill_matrix(matrix, NODE_COUNT, NUMBER_LIMIT, EDGE_EXISTS_CHANCE);

  if (NODE_COUNT <= 20)
    matrix.print();

  DistanceMatrix paths_fw(NODE_COUNT);
#ifdef FW
  run_solver_timed("Floyd-Warshall", FloydWarshallPathFinder(), matrix,
                   paths_fw);
  if (NODE_COUNT <= 20)
    paths_fw.print();
#endif

  DistanceMatrix paths_fw_omp(NODE_COUNT);
  run_solver_timed("Blocked Floyd-Warshall", BlockedFloydWarshallPathFinder(), matrix,
                   paths_fw_omp);
  if (NODE_COUNT <= 20)
    paths_fw_omp.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_fw_omp) <<
      std::endl;

#ifdef DIJKSTRA
  DistanceMatrix paths_d(NODE_COUNT);
  run_solver_timed("Dijkstra", DijkstraPathFinder(), matrix,
                   paths_d);
  if (NODE_COUNT <= 20)
    paths_d.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_d) <<
      std::endl;
#endif

  DistanceMatrix paths_dijkstra_omp(NODE_COUNT);
  run_solver_timed("OMP Dijkstra", DijkstraPathFinderOMP(), matrix,
                   paths_dijkstra_omp);
  if (NODE_COUNT <= 20)
    paths_dijkstra_omp.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_dijkstra_omp) <<
      std::endl;

  DistanceMatrix paths_dijkstra_omp_small(NODE_COUNT);
  run_solver_timed("OMP Dijkstra small", DijkstraPathFinderOMPSmall(), matrix,
                   paths_dijkstra_omp_small);
  if (NODE_COUNT <= 20)
    paths_dijkstra_omp_small.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_dijkstra_omp_small) <<
      std::endl;

  return 0;
}