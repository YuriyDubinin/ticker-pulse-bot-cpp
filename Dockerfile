# ---- Этап 1: Build ----
FROM --platform=linux/amd64 ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    g++-11 \
    curl \
    git \
    cmake \
    zip \
    unzip \
    tar \
    ninja-build \
    pkg-config \
    libssl-dev \
    ca-certificates \
    linux-libc-dev \
    autoconf \
    automake \
    libtool \
    bison \
    flex \
    && apt-get clean && rm -rf /var/lib/apt/lists/*


WORKDIR /app

# Клонирование и билд vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git vcpkg
WORKDIR /app/vcpkg
RUN ./bootstrap-vcpkg.sh

# Копирование только исходников и CMakeLists.txt
WORKDIR /app
COPY . .

# Установка зависимостей с помощью vcpkg
RUN /app/vcpkg/vcpkg install fmt zlib openssl nlohmann-json curl tgbot-cpp libpq

# Собираем проект
WORKDIR /app/build
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=/app/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
RUN make

# ---- Этап 2: Runtime ----
FROM --platform=linux/amd64 ubuntu:22.04

# Установка минимальных зависимостей
RUN apt-get update && apt-get install -y \
    libssl3 \
    ca-certificates && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копирование бинарника из билд-этапа
COPY --from=build /app/build/TickerPulseBot .

CMD ["./TickerPulseBot"]
