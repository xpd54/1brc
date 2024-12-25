#pragma once
#include <cstdint>
#include <string_view>
struct Station {
  int64_t sum_of_temp;
  int64_t number_of_record;

  int16_t maximum_temp;
  int16_t minimum_temp;
};

struct Station_Data {
  std::string_view station_name;
  int16_t station_temp_value;
  uint16_t hash;
};