#include "FlatMap.h"
#include "../Station.h"
#include <cstdint>
#include <string_view>
FlatMap::FlatMap() : _keys{}, _values{}, filled_indexes{}, hash_instance{} {}

std::pair<std::string_view *, bool> FlatMap::emplace(std::string_view key, const Station_INT &value) {}

Station_INT &FlatMap::operator[](const std::string_view &key) {}

Station_INT *FlatMap::find(const std::string_view &key) {
  uint16_t index = get_hash(key);
  while (!_keys[index].empty()) {
    if (_keys[index] == key)
      break;
    ++index;
  }
  return &_values[index];
}

std::vector<std::string_view> &FlatMap::sorted_keys() {}

size_t FlatMap::size() const { return filled_indexes.size(); }

void FlatMap::reserve(size_t size) { filled_indexes.reserve(size); }