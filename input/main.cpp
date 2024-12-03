#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#define CITY_LIST "measurement_cities.txt"
#define INPUT_LIST "measurement.txt"

/*Read the city list which only have single column with city name and push it to
 * a vector*/
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

  /*Create real distribution for city temparature range. City temparature would
   * vary from -99.9 to 99.9.*/
  std::uniform_real_distribution<double> real_dist(-99.9, 99.9);

  /*Create integral distribution for index of city to randomly pick on from city
   * list. We have 10000 unique city.*/
  std::uniform_int_distribution<int> integral_dis(0, 10000);

  std::vector<std::string> city_list = get_list_of_cities(CITY_LIST);

  /*How many entry to create.*/
  char *entry = argv[1];
  int number_of_entry = atoi(entry);

  /*Pick a random city and assign a random temparature.*/
  std::vector<double> input;
  input.reserve(number_of_entry);

  std::ofstream output_file(INPUT_LIST);
  for (int i = 0; i < number_of_entry; ++i) {
    output_file << city_list[integral_dis(generator)] << ';' << std::round(real_dist(generator) * 10) / 10 << '\n';
  }
  output_file.close();
  return 0;
}