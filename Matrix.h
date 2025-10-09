#pragma once

#include <cstddef>
#include <cstring>

#include <cstdio>

#include "Support.h"

#include <iostream>

template <typename Element>
class Matrix {
  const size_t m_Dimension;
  Element *const m_Data;

public:
  explicit Matrix(const size_t dimension) :
    m_Dimension(dimension),
    m_Data(static_cast<Element *>(safe_alloc(dimension * dimension,
                                             sizeof(Element)))) {
  }

  ~Matrix() {
    safe_free(m_Data);
  }

  Element *operator[](const size_t row) const {
    return m_Data + m_Dimension * row;
  }

  void zero_fill() const {
    std::memset(m_Data, 0, m_Dimension * m_Dimension * sizeof(Element));
  }

  void print() const {
    for (size_t row = 0; row < m_Dimension; row++) {
      for (size_t col = 0; col < m_Dimension; col++)
        if (col != m_Dimension - 1)
          std::cout << (*this)[row][col] << ' ';
      std::cout << '\n';
    }
    std::cout << std::flush;
  }
};