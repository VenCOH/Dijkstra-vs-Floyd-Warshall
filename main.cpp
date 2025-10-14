#include "DijkstraPathFinder.h"
#include "Matrix.h"

#include <cstdio>
#include <cstdint>

#include <random>
#include <chrono>
#include <iostream>

#include "Support.h"
#include "PathFinder.h"
#include "FloydWarshallPathFinder.h"
#include "DijkstraPathFinder.h"
#include "DijkstraThreadedPathFinder.h"
#include "FloydWarshallCudaFinder.cuh"

void run_solver_timed(const std::string &finder_name,
                      const PathFinder &path_finder,
                      const DistanceMatrix &transition_matrix,
                      const DistanceMatrix &paths) {
  std::cout << "Running path finder " << finder_name << std::endl;

  const std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();
  path_finder.find_paths(transition_matrix, paths);
  const std::chrono::steady_clock::time_point end_time =
      std::chrono::steady_clock::now();

  std::cout << "Execution time: " << std::chrono::duration_cast<
        std::chrono::microseconds>(end_time - start_time).count() << "[µs]" <<
      std::endl;
}

void random_fill_matrix(const DistanceMatrix &matrix, const node_t dimension,
                        const distance_t number_limit) {
  std::random_device random_generator;

  for (size_t row = 0; row < dimension; row++)
    for (size_t col = row + 1; col < dimension; col++) {
      matrix[row][col] = (static_cast<distance_t>(
                           random_generator() % static_cast<
                             std::random_device::result_type>(
                             number_limit))) / 100.0f;
      if (matrix[row][col] == 0)
        matrix[row][col] = DISTANCE_INFINITY;
      matrix[col][row] = matrix[row][col];
    }
}

int main() {
  constexpr size_t node_count = 100;
  constexpr size_t number_limit = 1000;

  const DistanceMatrix matrix(node_count, true);

  random_fill_matrix(matrix, node_count, number_limit);

  if (node_count <= 20)
    matrix.print();

  const DistanceMatrix paths_fw(node_count);
  run_solver_timed("Floyd-Warshall", FloydWarshallPathFinder(), matrix,
                   paths_fw);
  if (node_count <= 20)
    paths_fw.print();

  const DistanceMatrix paths_d(node_count);
  run_solver_timed("Dijkstra", DijkstraPathFinder(), matrix,
                   paths_d);
  if (node_count <= 20)
    paths_d.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_d) <<
      std::endl;

  const DistanceMatrix paths_dt(node_count);
  run_solver_timed("Dijkstra threaded", DijkstraThreadedPathFinder(), matrix,
                   paths_dt);
  if (node_count <= 20)
    paths_dt.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_dt) <<
      std::endl;

  const DistanceMatrix paths_dts(node_count);
  run_solver_timed("Dijkstra threaded smart",
                   DijkstraThreadedPathFinderSmart(20),
                   matrix,
                   paths_dts);
  if (node_count <= 20)
    paths_dts.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_dts) <<
      std::endl;

  const DistanceMatrix paths_fwc(node_count);
  run_solver_timed("Floyd-Warshall CUDA", FloydWarshallCudaFinder(),
                   matrix,
                   paths_fwc);
  if (node_count <= 20)
    paths_fwc.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_fwc) <<
      std::endl;

  return 0;
}