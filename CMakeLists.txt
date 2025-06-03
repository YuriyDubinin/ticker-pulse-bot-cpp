cmake_minimum_required(VERSION 3.10)

project(TickerPulseBot)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение toolchain-файла vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

# Определение платформы
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(VCPKG_TARGET_TRIPLET "arm64-osx")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Пути поиска пакетов
set(VCPKG_INSTALLED_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/${VCPKG_TARGET_TRIPLET}")
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_PATH}/share")
include_directories("${VCPKG_INSTALLED_PATH}/include")

# Поиск зависимостей
find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

# Поиск библиотеки tgbot-cpp вручную
find_library(TGBOT_CPP_LIB
    NAMES TgBot
    PATHS "${VCPKG_INSTALLED_PATH}/lib"
)

if(NOT TGBOT_CPP_LIB)
    message(FATAL_ERROR "tgbot-cpp (TgBot) library not found in ${VCPKG_INSTALLED_PATH}/lib")
endif()

# Заголовочные файлы
include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/config"
)

# Исходные файлы
add_executable(TickerPulseBot
    src/main.cpp
    src/thread_pool.cpp
    src/bot.cpp
    src/http_client.cpp
    src/utils.cpp
)

# Линковка зависимостей
target_link_libraries(TickerPulseBot
    fmt::fmt
    ZLIB::ZLIB
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    CURL::libcurl
    ${TGBOT_CPP_LIB}
)
