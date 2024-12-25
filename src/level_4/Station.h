#pragma once
#include <cstdint>
#include <string_view>
struct Station {
  double sum_of_temp;
  uint64_t number_of_record;

  float maximum_temp;
  float minimum_temp;
};

struct Station_INT {
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