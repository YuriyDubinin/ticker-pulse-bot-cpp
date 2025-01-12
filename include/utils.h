#ifndef UTILS
#define UTILS

#include <string>
#include <vector>

namespace utils {
    std::string toFixedDouble(double value, int precision);
    std::string stringifyStringsVectorToString(const std::vector<std::string>& stringVector, std::string delimiter = "");
    std::string capitalizeFirstLetter(const std::string string);
    std::vector<double> findCurrencyMinMax(const std::vector<std::vector<double>>& data);
};

#endif