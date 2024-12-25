#pragma once
#include <cstdint>
struct Station {
  int64_t sum_of_temp;
  int64_t number_of_record;

  int64_t maximum_temp;
  int64_t minimum_temp;
};