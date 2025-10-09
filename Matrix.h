#pragma once

#include <cstddef>

#include "Support.h"

#include <cstring>

class Matrix {
  const size_t m_Dimension;
  const size_t m_Element_size;
  std::byte *const m_Data;

public:
  Matrix(const std::size_t dimension, const std::size_t element_size) :
    m_Dimension(dimension), m_Element_size(element_size),
    m_Data(static_cast<std::byte*>(safe_alloc(dimension * dimension, element_size))) {
  }

  ~Matrix() {
    safe_free(m_Data);
  }

  const void* operator[] (const size_t row) const {
    return reinterpret_cast<void*>(m_Data + m_Dimension * row);
  }

  void zero_matrix() const {
    std::memset(m_Data, 0, m_Dimension * m_Dimension * m_Element_size);
  }
};
