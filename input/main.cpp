#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#define CITY_LIST "measurement_cities.txt"

std::vector<std::string> get_list_of_cities(std::string file_name) {
  std::ifstream file_stream(file_name);
  if (!file_stream) {
    std::cerr << "Can't read cities list." << '\n';
  }
  std::vector<std::string> city_list;
  std::string city;
  while (std::getline(file_stream, city, '\n')) {
    city_list.push_back(city);
  }
  return city_list;
}

int main(int argc, char **argv) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<double> real_dist(-99.9, 99.9);
  std::uniform_int_distribution<int> integral_dis(0, 10000);

  std::vector<std::string> city_list = get_list_of_cities(CITY_LIST);

  char *entry = argv[1];
  int number_of_entry = atoi(entry);

  std::vector<double> input;
  input.reserve(number_of_entry);
  for (int i = 0; i < number_of_entry; ++i) {
    input.push_back(std::round(real_dist(generator) * 10) / 10);
  }

  std::cout << input.size() << '\n';
  std::cout << input.back() << '\n';
  return 0;
}