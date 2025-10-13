#include "Matrix.h"

#include <cstdio>
#include <cstdint>

#include <random>
#include <chrono>
#include <iostream>

#include "Support.h"
#include "PathFinder.h"
#include "FloydWarshallPathFinder.h"


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
      matrix[row][col] = static_cast<distance_t>(
        random_generator() % static_cast<std::random_device::result_type>(
          number_limit));
      if (matrix[row][col] == 0)
        matrix[row][col] = DISTANCE_INFINITY;
      matrix[col][row] = matrix[row][col];
    }
}

int main() {
  constexpr size_t dimension = 10;
  constexpr size_t number_limit = 10;

  const DistanceMatrix matrix(dimension, true);

  random_fill_matrix(matrix, dimension, number_limit);

  matrix.print();

  const DistanceMatrix paths(dimension);

  run_solver_timed("Floyd-Warshall finder", FloydWarshallPathFinder(), matrix,
                   paths);

  paths.print();

  return 0;
}