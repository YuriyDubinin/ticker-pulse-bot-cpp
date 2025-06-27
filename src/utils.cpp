#include "utils.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace utils {
  std::string to_fixed_double(double value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    std::string result = oss.str();
    oss.str("");
    return result;
  };

  std::string stringify_strings_vector_to_string(const std::vector<std::string>& string_vector, std::string delimiter) {
    std::string result;

    for (size_t i = 0; i < string_vector.size(); ++i) {
      if (i > 0) {
        result += delimiter; // Добавляем разделитель перед каждым элементом, кроме первого
      }

      result += string_vector[i];
    }

    return result;
  };

  std::string capitalize_first_letter(const std::string string) {
    if (string.empty()) {
      return string;
    };

    std::string result = string;
    result[0]          = std::toupper(result[0]);

    return result;
  };

  std::vector<double> find_currency_min_max(const std::vector<std::vector<double>>& data) {
    double minVal = data[0][1];
    double maxVal = data[0][1];

    for (const auto& entry : data) {
      double value = entry[1];

      if (value < minVal) {
        minVal = value;
      }

      if (value > maxVal) {
        maxVal = value;
      }
    }

    return {minVal, maxVal};
  };
}; // namespace utils