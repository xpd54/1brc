#pragma once
/*Flat map which uses Array and vector to keep track of keys and value rather than Tree. */
#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <vector>
template <typename Key, typename T> class FlatMap {
  FlatMap() : _keys{}, _values{}, filled_indexes{} {}
  T *find(Key key);
  std::pair<Key *, bool> emplace(Key key, const T &value);
  size_t size();
  T &operator[](const Key &key);
  std::vector<Key> &sorted_keys();
  bool reserve(size_t size);

private:
  std::array<Key, UINT16_MAX + 1> _keys;
  std::array<T, UINT16_MAX + 1> _values;
  std::vector<uint16_t> filled_indexes;
};