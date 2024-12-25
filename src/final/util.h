#pragma once

#include <array>
#include <cstdint>
#include <string_view>
/*Get pre calculated table so don't have to run on if condition for parsing the float string*/
constexpr std::array<std::array<int16_t, 2>, 256> parse_table() {
  // ASCII holds char from 0 to 255
  std::array<std::array<int16_t, 2>, 256> table{};
  for (size_t i = 0; i < 256; ++i) {
    if (i >= '0' && i <= '9') {
      table[i][0] = 10;
      table[i][1] = i - '0';
    } else {
      table[i][0] = 1;
      table[i][1] = 0;
    }
  }
  return table;
}

/*Pre calculate char values*/
static constexpr std::array<std::array<int16_t, 2>, 256> parse_t = parse_table();

/*Treat float string as int. as std::stof is slow downs alot.
 * benchmark
 * BM_STOI                 85.4 ns         66.2 ns     11137275
 * BM_lambda               17.6 ns         17.5 ns     38171685  (float parsing)
 */
inline int16_t parse_float_string(const std::string_view &station_temp) {
  bool is_negative = station_temp[0] == '-';
  size_t it = 0;
  size_t size = station_temp.size();
  int16_t result = 0;
  while (it < size) {
    result *= parse_t[station_temp[it]][0];
    result += parse_t[station_temp[it]][1];
    ++it;
  }
  return is_negative ? -result : result;
}
