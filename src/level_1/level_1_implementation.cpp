#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "Station.h"

/* List of improvement from level_0
 * 1. Avoid making copy of station_map when returning from create_map_with_file and in print_out_output.
 * 2. Use unordered_map where emplace would be faster as no need to sort alphabitally (lexicographically)
 */

/*
 * Total run time
 * Number Of station:- 413
 * Time Taken in millisecond :- 319908ms
 * Time Taken in second :- 319s
 */

void create_map_with_file(std::ifstream &input_file_stream, std::unordered_map<std::string, Station> &station_map) {

  std::string station_name;
  std::string station_temp_string;
  while (std::getline(input_file_stream, station_name, ';') &&
         std::getline(input_file_stream, station_temp_string, '\n')) {
    float station_temp = std::stof(station_temp_string);
    if (!station_map.count(station_name)) {
      station_map.emplace(station_name, Station{station_temp, 1, station_temp, station_temp});
      continue;
    }

    std::unordered_map<std::string, Station>::iterator it = station_map.find(station_name);
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
void print_out_output(std::ostream &output_stream, std::unordered_map<std::string, Station> &station_map) {
  /* Station_map will have roughly 10000 entry so sorting and finding shouldn't implact performance*/
  std::vector<std::string> station_names;
  station_names.reserve(station_map.size());

  for (auto &[name, station] : station_map)
    station_names.push_back(name);
  std::sort(station_names.begin(), station_names.end());

  std::string first_delimiter = "";
  output_stream << std::setiosflags(output_stream.fixed | output_stream.showpoint) << std::setprecision(1);
  output_stream << '{';
  for (auto &name : station_names) {
    Station value = station_map[name];
    output_stream << std::exchange(first_delimiter, ", ");
    output_stream << name << '=';
    output_stream << value.minimum_temp << '/';
    output_stream << (value.sum_of_temp / value.number_of_record) << '/';
    output_stream << value.maximum_temp;
  }
  output_stream << '}';
}

/*Take input file name as an argument to test that, Use Sample
 * (input/measurement_100000.txt). Original 1B row can be generated with
 * input/main.cpp which takes 4-5 min*/

int main(int argc, char **argv) {

  auto start_time = std::chrono::high_resolution_clock::now();
  std::unordered_map<std::string, Station> measurement_map;
  std::ifstream measurement(argv[1]);
  if (!measurement) {
    std::cerr << "Error:- Can not read input file" << '\n';
  }
  create_map_with_file(measurement, measurement_map);
  print_out_output(std::cout, measurement_map);
  std::cout << '\n' << "Number Of station:- " << measurement_map.size() << '\n';
  auto end_time = std::chrono::high_resolution_clock::now();

  std::cout << "Time Taken in millisecond :- "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << '\n';

  std::cout << "Time Taken in second :- "
            << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << '\n';
  return 0;
}
