#pragma once

#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cmath>

#include <iostream>

#include "Support.h"

template <typename element_t, long double precision_multiplier>
class Matrix {
  element_t *const m_Data;

public:
  const size_t dimension;

  explicit Matrix(const size_t dimension, const bool zero = false) :
    dimension(dimension),
    m_Data(static_cast<element_t *>(safe_alloc(dimension * dimension,
                                               sizeof(element_t), zero))) {
  }

  Matrix(const Matrix<element_t, precision_multiplier> &that) : m_Data(
        static_cast<element_t *>(
          safe_alloc(
              that.dimension * that.
              dimension,
              sizeof(element_t)))),
    dimension(that.dimension) {
    memcpy(m_Data, that.m_Data,
           that.dimension * that.dimension * sizeof(element_t));
  }

  ~Matrix() {
    safe_free(m_Data);
  }

  bool operator==(const Matrix<element_t, precision_multiplier> &that) const {
    for (size_t i = 0; i < dimension * dimension; i++) {
      if (std::fabs(m_Data[i] - that.m_Data[i]) > precision_multiplier *
          std::max(m_Data[i], that.m_Data[i]))
        return false;
    }
    return true;
  }

  element_t *operator[](const size_t row) const {
    return m_Data + dimension * row;
  }

  void fill(const int byte) const {
    std::memset(m_Data, byte, dimension * dimension * sizeof(element_t));
  }

  void print() const {
    for (size_t row = 0; row < dimension; row++) {
      for (size_t col = 0; col < dimension; col++) {
        std::cout << (*this)[row][col];
        if (col != dimension - 1)
          std::cout << ' ';
      }
      std::cout << '\n';
    }
    std::cout << std::flush;
  }
};