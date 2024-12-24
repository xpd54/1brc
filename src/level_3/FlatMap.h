#pragma once
/*Flat map which uses Array and vector to keep track of keys and value rather than Tree. */
#include "../Station.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>
class FlatMap {
public:
  FlatMap();
  std::pair<Station_INT *, bool> emplace(std::string_view key, const Station_INT &value);
  Station_INT &operator[](const std::string_view &key);
  Station_INT *find(const std::string_view &key);
  std::vector<std::string_view> sorted_keys();
  size_t size() const;
  void reserve(size_t size);

  inline size_t count(const std::string_view &key) {
    uint16_t index = get_index(key);
    return _keys[index].empty() ? 0 : 1;
  }

private:
  inline uint16_t get_hash(const std::string_view &key) const { return hash_instance(key); }

  /*Use linear probing on collision of hash */
  inline size_t get_index(const std::string_view &key) const {
    uint16_t index = get_hash(key);
    while (!_keys[index].empty()) {
      if (_keys[index] == key)
        break;
      ++index;
    }
    return index;
  }

  std::array<std::string_view, UINT16_MAX + 1> _keys;
  std::array<Station_INT, UINT16_MAX + 1> _values;
  std::vector<uint16_t> filled_indexes;
  std::hash<std::string_view> hash_instance;
};