#include <string>
#include <vector>
#include <iomanip>
#include <sstream> 
#include "utils.h"

namespace utils {
    std::string toFixedDouble(double value, int precision) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        std::string result = oss.str();
        oss.str("");
        return result;
    };

    std::string stringifyStringsVectorToString(const std::vector<std::string>& stringVector, std::string delimiter) {
        std::string result;
        
        for (size_t i = 0; i < stringVector.size(); ++i) {
            if (i > 0) {
                result += delimiter;  // Добавляем разделитель перед каждым элементом, кроме первого
            }
            result += stringVector[i];
        }
        
        return result;
    };

    std::string capitalizeFirstLetter(const std::string string) {
        if (string.empty()) {
            return string;
        };
        std::string result = string;
        result[0] = std::toupper(result[0]);

        return result;
    };
};