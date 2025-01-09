#include <string>
#include <iomanip>
#include <sstream> 
#include "utils.h"

namespace utils {
    std::string toFixedDouble(double value, int precision) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        std::string string = oss.str();
        oss.str("");
        return string;
    }
}