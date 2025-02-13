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
 * Time Taken in millisecond :- 157615ms
 * Time Taken in second :- 157s
 */

using custom_unorder_map = FlatMap;
void create_map_with_file(const std::string_view &input_file_view, custom_unorder_map &station_map) {
  uint64_t size = input_file_view.size();
  uint64_t start = 0;
  uint64_t found = 0;
  while (start < size) {
    Station_Data station;
    station.hash = 0;
    /*We are accesing char already, Would like to get simple hash rather than uint64_t version from hash lib*/
    while (input_file_view[found] != ';') {
      station.hash = station.hash * 7 + input_file_view[found];
      ++found;
    }
    station.station_name = input_file_view.substr(start, found - start);
    start = found + 1;

    found = input_file_view.find('\n', start);
    station.station_temp_value = parse_float_string(input_file_view.substr(start, found - start));
    start = found + 1;
    /*Move found it by one*/
    ++found;
    if (!station_map.count(station.station_name, station.hash)) {
      station_map.insert(
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
void print_out_output(std::ostream &output_stream, custom_unorder_map &station_map) {
  station_map.sort_key_index();
  std::string first_delimiter = "";
  output_stream << std::setiosflags(output_stream.fixed | output_stream.showpoint) << std::setprecision(1);

  output_stream << '{';
  for (auto &index : station_map.filled_indexes) {
    auto &name = station_map._keys[index];
    auto &value = station_map._values[index];
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
  auto start_time = std::chrono::high_resolution_clock::now();

  MemoryMappedFile file(argv[1]);
  std::string_view measurement_view = file.fileArray();

  custom_unorder_map measurement_map;
  measurement_map.reserve(1000);
  create_map_with_file(measurement_view, measurement_map);

  /**/
  print_out_output(std::cout, measurement_map);

  /*---------------Print Time Load---------------*/
  std::cout << '\n' << "Number Of station:- " << measurement_map.size() << '\n';
  auto end_time = std::chrono::high_resolution_clock::now();

  std::cout << "Time Taken in millisecond :- "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << '\n';

  std::cout << "Time Taken in second :- "
            << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << '\n';
  return 0;
}