#include "FlatMap.h"
#include "Station.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string_view>
FlatMap::FlatMap() : _keys{}, _values{}, filled_indexes{} {}

void FlatMap::insert(std::string_view key, uint16_t &hash, const Station &value) {
  // check if there is a key already there if no insert
  uint16_t index = get_index(key, hash);
  filled_indexes.push_back(index);
  _keys[index] = key;
  _values[index] = value;
}

Station *FlatMap::find(const std::string_view &key, uint16_t &hash) {
  uint16_t index = get_index(key, hash);
  return &_values[index];
}

void FlatMap::sort_key_index() {
  auto comp = [&](uint16_t left, uint16_t rigth) { return _keys[left] < _keys[rigth]; };
  std::sort(filled_indexes.begin(), filled_indexes.end(), comp);
}

size_t FlatMap::size() const { return filled_indexes.size(); }

void FlatMap::reserve(size_t size) { filled_indexes.reserve(size); }