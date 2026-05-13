#pragma once

#include <stdint.h>
#include "ozz/base/io/archive.h"
#include "ozz/base/io/stream.h"

static_assert(sizeof(int) == 4, "expected 32-bit int");
static_assert(sizeof(unsigned int) == 4, "expected 32-bit unsigned int");

namespace ozz {
namespace io {

template <>
struct Extern<int> {
  static void Save(OArchive& _archive, const int* _values, size_t _count) {
    _archive.stream()->Write(_values, _count * sizeof(int));
  }

  static void Load(IArchive& _archive, int* _values, size_t _count, uint32_t) {
    _archive.stream()->Read(_values, _count * sizeof(int));
  }
};

template <>
struct Extern<unsigned int> {
  static void Save(OArchive& _archive, const unsigned int* _values, size_t _count) {
    _archive.stream()->Write(_values, _count * sizeof(unsigned int));
  }

  static void Load(IArchive& _archive, unsigned int* _values, size_t _count, uint32_t) {
    _archive.stream()->Read(_values, _count * sizeof(unsigned int));
  }
};

namespace internal {

template <>
struct Version<int> {
  enum { kValue = 0 };
};

template <>
struct Version<const int> {
  enum { kValue = 0 };
};

template <>
struct Version<unsigned int> {
  enum { kValue = 0 };
};

template <>
struct Version<const unsigned int> {
  enum { kValue = 0 };
};

} // namespace internal
} // namespace io
} // namespace ozz