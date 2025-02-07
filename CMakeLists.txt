cmake_minimum_required(VERSION 3.10)

# Устанавливаем политику CMP0167 на NEW
# cmake_policy(SET CMP0167 NEW)

project(TickerPulseBot)

# Стандарт C++ для проекта
set(CMAKE_CXX_STANDARD 17)

# Путь к файлу toolchain для vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Путь поиска пакетов 
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/share")

# Зависимости
find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

# Заголовочные файлы
include_directories(src)
include_directories("${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/include")
include_directories("${CMAKE_SOURCE_DIR}/config")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/vcpkg")

# Исходные файлы
add_executable(TickerPulseBot
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/thread_pool.cpp"
    "${CMAKE_SOURCE_DIR}/src/bot.cpp"
    "${CMAKE_SOURCE_DIR}/src/crypto_fetcher.cpp"
    "${CMAKE_SOURCE_DIR}/src/utils.cpp"
)

# Линк зависимостей с проектом
target_link_libraries(TickerPulseBot fmt::fmt)
target_link_libraries(TickerPulseBot ZLIB::ZLIB)
target_link_libraries(TickerPulseBot OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(TickerPulseBot nlohmann_json::nlohmann_json)
target_link_libraries(TickerPulseBot CURL::libcurl)

# Использование различных версий tgbot-cpp учитывая целевую платформу
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # MacOS arm64-osx
    message(STATUS "Building for macOS")
    target_link_libraries(TickerPulseBot "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/lib/libTgBot.a")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux arm64-linux
    message(STATUS "Building for Linux")
    target_link_libraries(TickerPulseBot "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-linux/lib/libTgBot.a")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()