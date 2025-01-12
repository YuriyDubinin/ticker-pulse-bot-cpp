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

    std::vector<double> findCurrencyMinMax(const std::vector<std::vector<double>>& data) {
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
};