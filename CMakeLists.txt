cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(TickerPulseBot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение платформы и тройника для vcpkg
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(VCPKG_TARGET_TRIPLET "arm64-osx")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Пути для vcpkg
set(VCPKG_INSTALLED_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/${VCPKG_TARGET_TRIPLET}")
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_PATH}/share")

# Поиск зависимостей
find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

# Поиск tgbot-cpp как библиотеку вручную
find_library(TGBOT_CPP_LIB
    NAMES TgBot
    PATHS "${VCPKG_INSTALLED_PATH}/lib"
)

if(NOT TGBOT_CPP_LIB)
    message(FATAL_ERROR "tgbot-cpp (TgBot) library not found in ${VCPKG_INSTALLED_PATH}/lib")
endif()

# Поиск исходников
file(GLOB SRC_SOURCES
    src/*.cpp
)

file(GLOB CONFIG_SOURCES
    config/*.cpp
)

set(ALL_SOURCES
    ${SRC_SOURCES}
    ${CONFIG_SOURCES}
)

add_executable(TickerPulseBot ${ALL_SOURCES})

# Директории для заголовков
target_include_directories(TickerPulseBot
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/config"
)

# Линковка библиотек
target_link_libraries(TickerPulseBot
    PRIVATE
    fmt::fmt
    ZLIB::ZLIB
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    CURL::libcurl
    ${TGBOT_CPP_LIB}
)
