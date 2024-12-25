#pragma once
#include <cstdint>
struct Station {
  double sum_of_temp;
  uint64_t number_of_record;

  float maximum_temp;
  float minimum_temp;
};