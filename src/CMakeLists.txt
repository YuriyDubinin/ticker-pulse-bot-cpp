cmake_minimum_required(VERSION 3.10)

# Устанавливаем политику CMP0167 на NEW
cmake_policy(SET CMP0167 NEW)

project(TickerPulseBot)

# Стандарт C++ для проекта
set(CMAKE_CXX_STANDARD 17)

# Путь к файлу toolchain для vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/src/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Путь поиска пакетов 
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/share")

# Зависимости
find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

# Заголовочные файлы
include_directories(src)
include_directories("${CMAKE_SOURCE_DIR}/src/utils")
include_directories("${CMAKE_SOURCE_DIR}/src/vcpkg/installed/arm64-osx/include")

# Исходные файлы
add_executable(TickerPulseBot
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/utils/ThreadPool/ThreadPool.cpp"
    "${CMAKE_SOURCE_DIR}/utils/Bot/Bot.cpp"
)

# Линк зависимостей с проектом
target_link_libraries(TickerPulseBot fmt::fmt)
target_link_libraries(TickerPulseBot ZLIB::ZLIB)
target_link_libraries(TickerPulseBot OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(TickerPulseBot "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/lib/libTgBot.a")