#include "../include/Matrix.h"
#include "../include/DijkstraPathFinder.h"

#include <cstdint>
#include <cstdio>

#include <random>
#include <chrono>
#include <iostream>

#include "../include/FloydWarshallPathFinder.h"
#include "../include/Support.h"
#include "../include/PathFinder.h"
// #include "DijkstraPathFinder.h"
// #include "DijkstraThreadedPathFinder.h"
// #include "FloydWarshallCudaFinder.cuh"

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
      static_cast<double>(std::numeric_limits<
        std::mt19937::result_type>::max());
  for (size_t row = 0; row < dimension; row++)
    for (size_t col = 0; col < dimension; col++) {
      if (row == col)
        matrix[row][col] = 0;
      else {
        if (generator() / max_random_number < edge_exists_chance)
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

int main() {
  constexpr size_t node_count = 4096;
  constexpr size_t number_limit = 1000;

  const DistanceMatrix matrix(node_count, true);

  random_fill_matrix(matrix, node_count, number_limit, 1);

  if (node_count <= 20)
    matrix.print();

  DistanceMatrix paths_fw(node_count);
  // run_solver_timed("Floyd-Warshall", FloydWarshallPathFinder(), matrix,
  //                  paths_fw);
  // if (node_count <= 20)
  //   paths_fw.print();

  DistanceMatrix paths_fw_omp(node_count);
  run_solver_timed("Blocked Floyd-Warshall", BlockedFloydWarshallPathFinder(), matrix,
                   paths_fw_omp);
  if (node_count <= 20)
    paths_fw_omp.print();
  std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_fw_omp) <<
      std::endl;

  // DistanceMatrix paths_d(node_count);
  // run_solver_timed("Dijkstra", DijkstraPathFinder(), matrix,
  //                  paths_d);
  // if (node_count <= 20)
  //   paths_d.print();
  // std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_d) <<
  //     std::endl;
  //
  // DistanceMatrix paths_dt(node_count);
  // run_solver_timed("Dijkstra threaded", DijkstraThreadedPathFinder(), matrix,
  //                  paths_dt);
  // if (node_count <= 20)
  //   paths_dt.print();
  // std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_dt) <<
  //     std::endl;
  //
  // DistanceMatrix paths_dts(node_count);
  // run_solver_timed("Dijkstra threaded smart",
  //                  DijkstraThreadedPathFinderSmart(10),
  //                  matrix,
  //                  paths_dts);
  // if (node_count <= 20)
  //   paths_dts.print();
  // std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_dts) <<
  //     std::endl;

  // const DistanceMatrix paths_fwc(node_count);
  // run_solver_timed("Floyd-Warshall CUDA", FloydWarshallCudaFinder(),
  //                  matrix,
  //                  paths_fwc);
  // if (node_count <= 20)
  //   paths_fwc.print();
  // std::cout << "Is same as Floyd-Warshall? " << (paths_fw == paths_fwc) <<
  //     std::endl;



  return 0;
}