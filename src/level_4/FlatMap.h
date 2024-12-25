#pragma once
/*Flat map which uses Array and vector to keep track of keys and value rather than Tree. */
#include "Station.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>
class FlatMap {
public:
  FlatMap();
  std::pair<Station *, bool> emplace(std::string_view key, uint64_t hash, const Station &value);
  const Station &get(const std::string_view &key, uint64_t hash);
  Station *find(const std::string_view &key, uint64_t hash);
  std::vector<std::string_view> sorted_keys();
  size_t size() const;
  void reserve(size_t size);

  inline bool count(const std::string_view &key, uint64_t hash) {
    uint16_t index = get_index(key, hash);
    return !_keys[index].empty();
  }

private:
  /*Use linear probing on collision of hash */
  inline size_t get_index(const std::string_view &key, uint64_t hash) {
    uint16_t index = hash;
    while (!_keys[index].empty()) {
      if (_keys[index] == key)
        break;
      ++index;
    }
    return index;
  }

  std::array<std::string_view, UINT16_MAX + 1> _keys;
  std::array<Station, UINT16_MAX + 1> _values;
  std::vector<uint16_t> filled_indexes;
};