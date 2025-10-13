#pragma once

#include <cstddef>
#include <cstring>
#include <cstdio>

#include <iostream>

#include "Support.h"

template <typename Element>
class Matrix {
  Element *const m_Data;

public:
  const size_t dimension;

  explicit Matrix(const size_t dimension, const bool zero = false) :
    dimension(dimension),
    m_Data(static_cast<Element *>(safe_alloc(dimension * dimension,
                                             sizeof(Element), zero))) {
  }

  Matrix(const Matrix<Element> &that) : m_Data(
                                            static_cast<Element *>(safe_alloc(
                                                that.dimension * that.dimension,
                                                sizeof(Element)))),
                                        dimension(that.dimension) {
    memcpy(m_Data, that.m_Data,
           that.dimension * that.dimension * sizeof(Element));
  }

  ~Matrix() {
    safe_free(m_Data);
  }

  Element *operator[](const size_t row) const {
    return m_Data + dimension * row;
  }

  void fill(const int byte) const {
    std::memset(m_Data, byte, dimension * dimension * sizeof(Element));
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