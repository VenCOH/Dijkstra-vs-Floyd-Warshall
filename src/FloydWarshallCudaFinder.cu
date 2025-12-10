#include "../include/FloydWarshallCudaFinder.cuh"

#define THREADS_PER_BLOCK 256

__global__ void simple_kernel(
    const distance_t *const __restrict input_matrix,
    distance_t *const __restrict output_matrix,
    node_t matrix_size,
    node_t k) {
  const node_t thread_number = blockIdx.x * blockDim.x + threadIdx.x;
  if (thread_number >= matrix_size * matrix_size)
    return;
  const distance_t original_distance = input_matrix[thread_number];
  const node_t y = thread_number / matrix_size;
  const node_t x = thread_number % matrix_size;

  const distance_t new_distance =
      input_matrix[y * matrix_size + k] + input_matrix[
        k * matrix_size + x];

  if (original_distance > new_distance)
    output_matrix[thread_number] = new_distance;
  else
    output_matrix[thread_number] = original_distance;
}

void FloydWarshallCudaFinder::find_paths(
    const DistanceMatrix &transition_matrix, DistanceMatrix &paths) const {
  const size_t number_of_nodes = transition_matrix.dimension;
  const size_t number_of_blocks =
      (number_of_nodes * number_of_nodes - 1) / THREADS_PER_BLOCK + 1;

  void *matrix1, *matrix2;
  cudaMallocManaged(&matrix1,
                    number_of_nodes * number_of_nodes * sizeof(distance_t));
  cudaMallocManaged(&matrix2,
                    number_of_nodes * number_of_nodes * sizeof(distance_t));
  cudaMemcpy(matrix1, transition_matrix[0],
             number_of_nodes * number_of_nodes * sizeof(distance_t),
             cudaMemcpyDefault);
  for (node_t k = 0; k < number_of_nodes; k++) {
    simple_kernel<<<number_of_blocks, THREADS_PER_BLOCK>>>(
        static_cast<distance_t *>(matrix1), static_cast<distance_t *>(matrix2),
        number_of_nodes, k);
    void *tmp_matrix = matrix1;
    matrix1 = matrix2;
    matrix2 = tmp_matrix;
  }
  cudaDeviceSynchronize();
  cudaMemcpy(paths[0], matrix1,
             number_of_nodes * number_of_nodes * sizeof(distance_t),
             cudaMemcpyDefault);
}