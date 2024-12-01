#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
int main(int argc, char *argv[]) {
  std::ifstream file_stream("./measurement_cities.txt");
  if (!file_stream) {
    std::cerr << "Can't read cities list." << '\n';
    return 1;
  }
  std::vector<std::string> city_list;
  std::string city;
  while (std::getline(file_stream, city, '\n')) {
    city_list.push_back(city);
  }
  for (auto &value : city_list) {
    assert(value.size() <= 100 && !value.empty());
  }
  std::cout << city_list.size() << '\n';
  return 0;
}