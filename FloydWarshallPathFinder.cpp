#include <cstddef>

#include "Support.h"
#include "FloydWarshallPathFinder.h"
#include <omp.h>

void FloydWarshallPathFinder::find_paths(
    const DistanceMatrix &transition_matrix, DistanceMatrix &paths) const {
  const size_t dimension = transition_matrix.dimension;
  DistanceMatrix matrix1{transition_matrix}, matrix2{dimension};

  DistanceMatrix *current = &matrix1, *next = &matrix2;

  for (size_t transition_node = 0; transition_node < dimension; transition_node
       ++) {
    for (size_t start_node = 0; start_node < dimension; start_node++)
      for (size_t end_node = 0; end_node < dimension; end_node++) {
        if (const distance_t potential_length =
              (*current)[start_node][transition_node] + (*current)[
                transition_node][end_node];
          (*current)[start_node][end_node] > potential_length && (*current)[
            start_node][transition_node] != DISTANCE_INFINITY && (*current)[
            transition_node][end_node] != DISTANCE_INFINITY)
          (*next)[start_node][end_node] = potential_length;
        else
          (*next)[start_node][end_node] = (*current)[start_node][end_node];
      }
    DistanceMatrix *tmp = current;
    current = next;
    next = tmp;
  }

  memcpy(paths[0], (*current)[0], dimension * dimension * sizeof(*paths[0]));
}

#define BLOCK_SIZE 16

#define MIN(x, y) ((x) < (y) ? (x) : (y))

void BlockedFloydWarshallPathFinder::find_paths(
    const DistanceMatrix &transition_matrix, DistanceMatrix &paths) const {
  const size_t node_count = transition_matrix.dimension;
  Matrix matrix{transition_matrix};

  omp_set_num_threads(omp_get_num_procs());
  omp_set_max_active_levels(2);
  omp_set_nested(1);

  #pragma omp parallel default(shared)
  {
    #pragma omp single
    for (size_t block = 0; block < node_count/BLOCK_SIZE; block++) {
      const size_t begin = block*BLOCK_SIZE;
      const size_t end = (block + 1) * BLOCK_SIZE;

      // independent blocks
      // #pragma omp parallel for schedule(static, 16) collapse(3)
      for (size_t k = begin; k < end; k++)
        for (size_t i = begin; i < end; i++)
          for (size_t j = begin; j < end; j++)
            matrix[i][j] = MIN(matrix[i][j], matrix[i][k] + matrix[k][j]);

      // i-aligned slightly dependent blocks
      // #pragma omp parallel for schedule(guided)
      for (size_t i_block = 0; i_block < node_count/BLOCK_SIZE; i_block++) {
        const size_t i_begin = i_block * BLOCK_SIZE;
        const size_t i_end = (i_block + 1) * BLOCK_SIZE;
        for (size_t k = begin; k < end; k++)
          for (size_t i = begin; i < end; i++)
            for (size_t j = i_begin; j < i_end; j++)
              matrix[i][j] = MIN(matrix[i][j], matrix[i][k] + matrix[k][j]);
      }

      // j-aligned slightly dependent blocks
      // #pragma omp parallel for schedule(guided)
      for (size_t j_block = 0; j_block < node_count/BLOCK_SIZE; j_block++) {
        const size_t j_begin = j_block * BLOCK_SIZE;
        const size_t j_end = (j_block + 1) * BLOCK_SIZE;
        for (size_t k = begin; k < end; k++)
          for (size_t i = j_begin; i < j_end; i++)
            for (size_t j = begin; j < end; j++)
              matrix[i][j] = MIN(matrix[i][j], matrix[i][k] + matrix[k][j]);
      }

      // doubly dependent blocks
      #pragma omp parallel for collapse(2) schedule(guided)
      for (size_t i_block = 0; i_block < node_count/BLOCK_SIZE; i_block++) {
        const size_t i_begin = i_block * BLOCK_SIZE;
        const size_t i_end = (i_block + 1) * BLOCK_SIZE;
        for (size_t j_block = 0; j_block < node_count/BLOCK_SIZE; j_block++) {
          const size_t j_begin = j_block * BLOCK_SIZE;
          const size_t j_end = (j_block + 1) * BLOCK_SIZE;
          for (size_t i = j_begin; i < j_end; i++)
            for (size_t j = i_begin; j < i_end; j++)
              for (size_t k = begin; k < end; k++)
                matrix[i][j] = MIN(matrix[i][j], matrix[i][k] + matrix[k][j]);
        }
      }
    }
  }

  memcpy(paths[0], matrix[0], node_count * node_count * sizeof(*paths[0]));
}
