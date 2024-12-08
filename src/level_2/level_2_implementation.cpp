#include "../Station.h"
#include "MemoryMappedFile.h"
#include "utility.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

/* List of improvement from level_1
 * 1. We will use mmap to read the file in memory rather than reading from fstream
 * 2. As file is already mapped in memory we will use string_view to read the value into our station map
 */

/*
 * Total run time
 * Number Of station:- 9671
 * Time Taken in millisecond :- 260352ms
 * Time Taken in second :- 260s
 */

inline int64_t parse_float_string(const std::string_view &station_temp) {
  bool is_negative = station_temp[0] == '-';
  size_t it = 0;
  int result = 0;
  if (is_negative) {
    it = 1;
  }
  size_t size = station_temp.size();
  while (it < size) {
    if (station_temp[it] != '.')
      result = result * 10 + (station_temp[it] - '0');
    ++it;
  }
  return is_negative ? -result : result;
}

using custom_unorder_map = std::unordered_map<std::string_view, Station_INT, decltype(hash), decltype(equal)>;

void create_map_with_file(const std::string_view &input_file_view, custom_unorder_map &station_map) {
  std::string_view station_name;
  std::string_view station_temp_string;
  uint64_t size = input_file_view.size();
  uint64_t start = 0;
  uint64_t found = input_file_view.size();
  while (start < size) {
    found = input_file_view.find(';', start);
    station_name = input_file_view.substr(start, found - start);
    start = found + 1;

    found = input_file_view.find('\n', start);
    station_temp_string = input_file_view.substr(start, found - start);
    start = found + 1;

    int64_t station_temp = parse_float_string(station_temp_string);

    if (!station_map.count(station_name)) {
      station_map.emplace(station_name, Station_INT{station_temp, 1, station_temp, station_temp});
      continue;
    }

    auto it = station_map.find(station_name);
    it->second.sum_of_temp += station_temp;
    it->second.number_of_record++;
    it->second.maximum_temp = std::max(it->second.maximum_temp, station_temp);
    it->second.minimum_temp = std::min(it->second.minimum_temp, station_temp);
  }
}

/*
  Output formate is defined as following
 *{Abha=-23.0/18.0/59.2, Abidjan=-16.2/26.0/67.3, Abéché=-10.0/29.4/69.0,
 * Accra=-10.1/26.4/66.4, Addis Ababa=-23.7/16.0/67.0,
 Adelaide=-27.8/17.3/58.5,
 * ...}
 * <min>/<avrage>/<max>
 */
void print_out_output(std::ostream &output_stream, custom_unorder_map &station_map) {
  /* Station_map will have roughly 10000 entry so sorting and finding shouldn't implact performance*/
  std::vector<std::string_view> station_names;
  station_names.reserve(station_map.size());

  for (auto &[name, station] : station_map)
    station_names.push_back(name);
  std::sort(station_names.begin(), station_names.end());

  std::string first_delimiter = "";
  output_stream << std::setiosflags(output_stream.fixed | output_stream.showpoint) << std::setprecision(1);
  output_stream << '{';
  for (auto &name : station_names) {
    Station_INT value = station_map[name];
    output_stream << std::exchange(first_delimiter, ", ");
    output_stream << name << '=';
    output_stream << (value.minimum_temp / 10.0) << '/';
    output_stream << (value.sum_of_temp / (value.number_of_record * 10.0)) << '/';
    output_stream << (value.maximum_temp / 10.0);
  }
  output_stream << '}';
}

/*Take input file name as an argument to test that, Use Sample
 * (input/measurement_100000.txt). Original 1B row can be generated with
 * input/main.cpp which takes 4-5 min*/

int main(int argc, char **argv) {

  auto start_time = std::chrono::high_resolution_clock::now();

  MemoryMappedFile file(argv[1]);
  std::string_view measurement_view = file.fileArray();

  custom_unorder_map measurement_map(100000, hash, equal);
  create_map_with_file(measurement_view, measurement_map);
  print_out_output(std::cout, measurement_map);
  std::cout << '\n' << "Number Of station:- " << measurement_map.size() << '\n';
  auto end_time = std::chrono::high_resolution_clock::now();

  std::cout << "Time Taken in millisecond :- "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << '\n';

  std::cout << "Time Taken in second :- "
            << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << '\n';
  return 0;
}