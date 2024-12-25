#pragma once
#include <cstdint>
struct Station {
  double sum_of_temp;
  uint64_t number_of_record;

  float maximum_temp;
  float minimum_temp;
};

struct Station_INT {
  int64_t sum_of_temp;
  int64_t number_of_record;

  int64_t maximum_temp;
  int64_t minimum_temp;
};