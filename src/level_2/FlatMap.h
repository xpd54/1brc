#pragma once
/*Flat map which uses Array and vector to keep track of keys and value rather than Tree. */
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
template <typename Key, typename T> class FlatMap {
  FlatMap();
  T *find(Key key);
  std::pair<Key *, bool> emplace(Key key, const T &value);
  size_t size();
  T &operator[](const Key &key);
  std::vector<Key> &sorted_keys();

private:
  std::array<Key, UINT16_MAX> _keys;
  std::array<T, UINT16_MAX> _values;
  std::vector<size_t> filled_indexes;
};