# FROM ubuntu:22.04
FROM --platform=linux/arm64 ubuntu:22.04

# Установка необходимых зависимостей для системы
RUN apt-get update && apt-get install -y \
    build-essential \
    g++-11 \
    curl \
    zip \
    unzip \
    tar \
    git \
    cmake \
    ninja-build \
    pkg-config \
    libssl-dev \
    ca-certificates && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Настройка vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git vcpkg
WORKDIR /app/vcpkg
RUN ./bootstrap-vcpkg.sh

# Установка зависимостей через vcpkg
RUN ./vcpkg install fmt zlib openssl nlohmann-json curl tgbot-cpp

# Создание директории для сборки
WORKDIR /app/build

# Сборка проекта с использованием CMake и toolchain vcpkg
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=/app/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN make

# Определение команды для запуска бота
CMD ["./TickerPulseBot"]
