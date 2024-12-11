#pragma once
/*Flat map which uses Array and vector to keep track of keys and value rather than Tree. */
#include "../Station.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>
class FlatMap {
  FlatMap();
  std::pair<std::string_view *, bool> emplace(std::string_view key, const Station_INT &value);
  Station_INT &operator[](const std::string_view &key);
  Station_INT *find(const std::string_view &key);
  std::vector<std::string_view> &sorted_keys();
  size_t size() const;
  void reserve(size_t size);

private:
  inline uint16_t get_hash(const std::string_view &key) const {
    uint16_t index = hash_instance(key);
    return index;
  }

  std::array<std::string_view, UINT16_MAX + 1> _keys;
  std::array<Station_INT, UINT16_MAX + 1> _values;
  std::vector<uint16_t> filled_indexes;
  std::hash<std::string_view> hash_instance;
};