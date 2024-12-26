#include "FlatMap.h"
#include "MemoryMappedFile.h"
#include "Station.h"
#include "util.h"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

/* List of improvement from level_1
 * 1. Use constexper for parsing the float
 * 2. Improve some branch prediction by reducing if/else
 */

/*
 * Total run time
 * Number Of station:- 413
 * Number of Thread :- 32
 * Section Size :- 512
 * Time Taken in millisecond :- 27440ms
 * Time Taken in second :- 25s
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

std::unordered_map<std::string, Station> process_section_array(MemoryMappedFile &file, size_t number_of_thread,
                                                               size_t section_size) {
  std::vector<std::thread> threads;
  threads.reserve(number_of_thread);
  std::vector<custom_unorder_map> map_array(number_of_thread);
  map_array.reserve(number_of_thread);

  for (size_t it = 0; it < number_of_thread; ++it) {
    threads.push_back(std::thread([&, i = it]() {
      auto section = file.next_section_array(section_size);
      while (section.size()) {
        create_map_with_file(section, map_array[i]);
        section = file.next_section_array(section_size);
      }
    }));
  }

  for (auto &thread : threads)
    thread.join();
  // merge map_array into single map to print out
  std::unordered_map<std::string, Station> merged_map;
  for (auto &map : map_array) {
    for (auto &index : map.filled_indexes) {
      auto it = merged_map.find(map._keys[index]);
      if (it == merged_map.end()) {
        merged_map.insert_or_assign(map._keys[index], map._values[index]);
      } else {
        it->second.number_of_record += map._values[index].number_of_record;
        it->second.sum_of_temp += map._values[index].sum_of_temp;
        it->second.minimum_temp = std::min(it->second.minimum_temp, map._values[index].minimum_temp);
        it->second.maximum_temp = std::max(it->second.maximum_temp, map._values[index].maximum_temp);
      }
    }
  }
  return merged_map;
}

void print_out_output(std::ostream &output_stream, std::unordered_map<std::string, Station> &station_map) {
  std::vector<std::string> station_list;
  station_list.reserve(station_map.size());
  for (auto &value : station_map) {
    station_list.push_back(value.first);
  }

  std::sort(station_list.begin(), station_list.end());
  std::string first_delimiter = "";
  output_stream << std::setiosflags(output_stream.fixed | output_stream.showpoint) << std::setprecision(1);

  output_stream << '{';
  for (auto &name : station_list) {
    auto &value = station_map[name];
    output_stream << std::exchange(first_delimiter, ", ");
    output_stream << name << '=';
    output_stream << (value.minimum_temp / 10.0) << '/';
    output_stream << (value.sum_of_temp / 10.0) / value.number_of_record << '/';
    output_stream << (value.maximum_temp / 10.0);
  }
  output_stream << '}';
}

int main(int argc, char **argv) {
  auto start_time = std::chrono::high_resolution_clock::now();
  size_t thread_count = std::stoi(argv[2]);
  size_t section_size = std::stoi(argv[3]);

  MemoryMappedFile file(argv[1]);
  std::unordered_map<std::string, Station> merged_map = process_section_array(file, thread_count, section_size);
  /**/
  print_out_output(std::cout, merged_map);

  /*---------------Print Time Load---------------*/
  std::cout << '\n' << "Number Of station:- " << merged_map.size() << '\n';
  std::cout << "Number of Thread :- " << thread_count << '\n';
  std::cout << "Section Size :- " << section_size << '\n';
  auto end_time = std::chrono::high_resolution_clock::now();

  std::cout << "Time Taken in millisecond :- "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << '\n';

  std::cout << "Time Taken in second :- "
            << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << '\n';
  return 0;
}