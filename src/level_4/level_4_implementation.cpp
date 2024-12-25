#include "FlatMap.h"
#include "MemoryMappedFile.h"
#include "Station.h"
#include "util.h"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

/* List of improvement from level_1
 * 1. Use constexper for parsing the float
 * 2. Improve some branch prediction by reducing if/else
 */

/*
 * Total run time
 * Number Of station:- 413
 * Time Taken in millisecond :- 165393ms
 * Time Taken in second :- 165s
 */

// using custom_unorder_map = std::unordered_map<std::string_view, Station, decltype(hash), std::equal_to<>>;
using custom_unorder_map = FlatMap;
void create_map_with_file(const std::string_view &input_file_view, custom_unorder_map &station_map,
                          const std::hash<std::string_view> &hash) {
  uint64_t size = input_file_view.size();
  uint64_t start = 0;
  uint64_t found = input_file_view.size();
  while (start < size) {
    Station_Data station;
    found = input_file_view.find(';', start);
    station.station_name = input_file_view.substr(start, found - start);
    station.hash = hash(station.station_name);
    start = found + 1;

    found = input_file_view.find('\n', start);
    station.station_temp_value = parse_float_string(input_file_view.substr(start, found - start));
    start = found + 1;
    if (!station_map.count(station.station_name, station.hash)) {
      station_map.emplace(
          station.station_name, station.hash,
          Station{station.station_temp_value, 1, station.station_temp_value, station.station_temp_value});
      continue;
    }

    auto it = station_map.find(station.station_name, station.hash);
    it->sum_of_temp += station.station_temp_value;
    it->number_of_record++;

    if (it->minimum_temp > station.station_temp_value)
      it->minimum_temp = station.station_temp_value;
    else if (it->maximum_temp < station.station_temp_value)
      it->maximum_temp = station.station_temp_value;
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
void print_out_output(std::ostream &output_stream, custom_unorder_map &station_map,
                      const std::hash<std::string_view> &hash) {
  /* Station_map will have roughly 10000 entry so sorting and finding shouldn't implact performance*/
  std::vector<std::string_view> station_names;
  station_names = station_map.sorted_keys();

  std::string first_delimiter = "";
  output_stream << std::setiosflags(output_stream.fixed | output_stream.showpoint) << std::setprecision(1);
  output_stream << '{';
  for (auto &name : station_names) {
    Station value = station_map.get(name, hash(name));
    output_stream << std::exchange(first_delimiter, ", ");
    output_stream << name << '=';
    output_stream << (value.minimum_temp / 10.0) << '/';
    output_stream << (value.sum_of_temp / 10.0) / value.number_of_record << '/';
    output_stream << (value.maximum_temp / 10.0);
  }
  output_stream << '}';
}

/*Take input file name as an argument to test that, Use Sample
 * (input/measurement_100000.txt). Original 1B row can be generated with
 * input/main.cpp which takes 4-5 min*/

int main(int argc, char **argv) {
  std::hash<std::string_view> hash_instance;
  auto start_time = std::chrono::high_resolution_clock::now();

  MemoryMappedFile file(argv[1]);
  std::string_view measurement_view = file.fileArray();

  custom_unorder_map measurement_map;
  measurement_map.reserve(1000);
  create_map_with_file(measurement_view, measurement_map, hash_instance);
  print_out_output(std::cout, measurement_map, hash_instance);
  std::cout << '\n' << "Number Of station:- " << measurement_map.size() << '\n';
  auto end_time = std::chrono::high_resolution_clock::now();

  std::cout << "Time Taken in millisecond :- "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << '\n';

  std::cout << "Time Taken in second :- "
            << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << '\n';
  return 0;
}