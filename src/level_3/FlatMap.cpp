#include "FlatMap.h"
#include "../Station.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string_view>
FlatMap::FlatMap() : _keys{}, _values{}, filled_indexes{}, hash_instance{} {}

std::pair<Station_INT *, bool> FlatMap::emplace(std::string_view key, const Station_INT &value) {
  // check if there is a key already there if no insert
  uint16_t index = get_index(key);
  if (_keys[index].empty()) {
    filled_indexes.push_back(index);
    _keys[index] = key;
    _values[index] = value;
    return {&_values[index], true};
  }

  // if there is a key return the value at that index
  return {&_values[index], false};
}

Station_INT &FlatMap::operator[](const std::string_view &key) {
  uint16_t index = get_index(key);
  return _values[index];
}

Station_INT *FlatMap::find(const std::string_view &key) {
  uint16_t index = get_index(key);
  return &_values[index];
}

std::vector<std::string_view> FlatMap::sorted_keys() {
  auto comp = [&](uint16_t left, uint16_t rigth) { return _keys[left] < _keys[rigth]; };
  std::sort(filled_indexes.begin(), filled_indexes.end(), comp);
  std::vector<std::string_view> sorted_keys;
  for (const auto &index : filled_indexes) {
    sorted_keys.push_back(_keys[index]);
  }
  return sorted_keys;
}

size_t FlatMap::size() const { return filled_indexes.size(); }

void FlatMap::reserve(size_t size) { filled_indexes.reserve(size); }