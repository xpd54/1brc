#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>

#include "../Station.h"

std::map<std::string, Station> create_map_with_file(
    std::ifstream &input_file_stream) {
  std::map<std::string, Station> station_map;
  std::string station_name;
  std::string station_temp_string;
  while (std::getline(input_file_stream, station_name, ';') &&
         std::getline(input_file_stream, station_temp_string, '\n')) {
    float station_temp = std::stof(station_temp_string);
    if (!station_map.count(station_name)) {
      station_map.emplace(station_name,
                          Station{station_temp, 1, station_temp, station_temp});
      continue;
    }

    std::map<std::string, Station>::iterator it =
        station_map.find(station_name);
    it->second.sum_of_temp += station_temp;
    it->second.number_of_record++;
    it->second.maximum_temp = std::max(it->second.maximum_temp, station_temp);
    it->second.minimum_temp = std::min(it->second.minimum_temp, station_temp);
  }
  return station_map;
}

/*
  Output formate is defined as following
 *{Abha=-23.0/18.0/59.2, Abidjan=-16.2/26.0/67.3, Abéché=-10.0/29.4/69.0,
 * Accra=-10.1/26.4/66.4, Addis Ababa=-23.7/16.0/67.0,
 Adelaide=-27.8/17.3/58.5,
 * ...}
 * <min>/<avrage>/<max>
 */
void print_out_output(std::ostream &output_stream,
                      std::map<std::string, Station> station_map) {
  std::string first_delimiter = "";
  output_stream << std::setiosflags(output_stream.fixed |
                                    output_stream.showpoint)
                << std::setprecision(1);
  output_stream << '{';
  for (auto value : station_map) {
    output_stream << std::exchange(first_delimiter, ", ");
    output_stream << value.first << '=';
    output_stream << value.second.minimum_temp << '/';
    output_stream << (value.second.sum_of_temp / value.second.number_of_record)
                  << '/';
    output_stream << value.second.maximum_temp;
  }
  output_stream << '}';
}

int main() {
  std::ifstream measurement("../input/measurement_100000.txt");
  if (!measurement) {
    std::cerr << "Error:- Can not read input file" << '\n';
  }
  std::map<std::string, Station> measurement_map =
      create_map_with_file(measurement);
  print_out_output(std::cout, measurement_map);
  std::cout << '\n' << "Number Of station:- " << measurement_map.size() << '\n';
  return 0;
}
