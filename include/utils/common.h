#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace common {
  std::string         to_fixed_double(double value, int precision);
  std::string         stringify_strings_vector_to_string(const std::vector<std::string>& string_vector,
                                                         std::string                     delimiter = "");
  std::string         capitalize_first_letter(const std::string string);
  std::vector<double> find_currency_min_max(const std::vector<std::vector<double>>& data);
}; // namespace common

#endif