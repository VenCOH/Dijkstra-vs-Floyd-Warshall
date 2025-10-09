#include "Matrix.h"

#include <cstdio>
#include <cstdint>

#include <random>
#include <chrono>
#include <iostream>

#include "Support.h"
#include "PathFinder.h"


void run_solver_timed(const std::string &finder_name, PathFinder &path_finder,
                      const TransitionMatrix &transition_matrix, Paths &paths) {
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

void random_fill_matrix(const Matrix<unsigned> &matrix, const size_t dimension,
                        const unsigned number_limit) {
  std::random_device random_generator;

  for (size_t row = 0; row < dimension; row++)
    for (size_t col = 0; col < dimension; col++)
      matrix[row][col] = random_generator() % number_limit;
}

int main() {
  constexpr size_t dimension = 1000;

  return 0;
}