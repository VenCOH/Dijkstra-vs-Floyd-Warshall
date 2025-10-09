#pragma once

#include <cstddef>

#include "Support.h"

#include <cstring>

template <typename Element>
class Matrix {
  const size_t m_Dimension;
  Element *const m_Data;

public:
  Matrix(const size_t dimension, const size_t element_size) :
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

  void zero_matrix() const {
    std::memset(m_Data, 0, m_Dimension * m_Dimension * sizeof(Element));
  }
};