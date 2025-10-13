#include <cstddef>

#include "Support.h"
#include "FloydWarshallPathFinder.h"

void FloydWarshallPathFinder::find_paths(const TMatrix &transition_matrix,
                                         const TMatrix &paths) const {
  const size_t dimension = transition_matrix.dimension;
  TMatrix matrix1{transition_matrix}, matrix2{dimension};

  TMatrix *current = &matrix1, *next = &matrix2;

  for (size_t transition_node = 0; transition_node < dimension; transition_node
       ++) {
    for (size_t start_node = 0; start_node < dimension; start_node++)
      for (size_t end_node = 0; end_node < dimension; end_node++) {
        if (const unsigned potential_length =
              (*current)[start_node][transition_node] + (*current)[
                transition_node][end_node];
          (*current)[start_node][end_node] > potential_length && (*current)[
            start_node][transition_node] != T_INFINITY && (*current)[
            transition_node][end_node] != T_INFINITY)
          (*next)[start_node][end_node] = potential_length;
        else
          (*next)[start_node][end_node] = (*current)[start_node][end_node];
      }
    TMatrix *tmp = current;
    current = next;
    next = tmp;
  }

  memcpy(paths[0], (*current)[0], dimension * dimension * sizeof(*paths[0]));
}