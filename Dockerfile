FROM ubuntu:22.04

# OS packages
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    g++

# Copying sources into a container
COPY src /src

# Working directory
WORKDIR /src

# Build directory
RUN mkdir /build

# Building
WORKDIR /build
RUN cmake .. && make

# Start command when starting a container
CMD ["./TickerPulseBot"]
