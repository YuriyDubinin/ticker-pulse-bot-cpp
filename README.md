# ticker-pulse-bot-cpp v.1.0.3

**ticker-pulse-bot** is a Telegram bot designed for monitoring cryptocurrency markets. The bot helps track specific cryptocurrencies and signals when predefined threshold values (minimums and maximums) are reached. It also provides real-time information about cryptocurrencies and their prices. This solution is perfect for anyone who wants to stay updated on the crypto market and receive notifications about significant changes.

## Features

- **Cryptocurrency Monitoring**: The bot tracks the current prices of cryptocurrencies and compares them against predefined thresholds.
- **Alerts**: If a cryptocurrency's price reaches a set minimum or maximum, the bot sends a notification to a Telegram channel or individual chat.
- **Daily Updates**: The bot collects data on cryptocurrencies daily, including their highest and lowest values for the past month.
- **Information Retrieval**: Users can request real-time cryptocurrency data via bot commands.

## Capabilities

- Set threshold values for cryptocurrencies.
- Daily updates on cryptocurrency data.
- Retrieve real-time data on cryptocurrencies at any time.
- Simple to use and configure.

## Installation

To get started with **ticker-pulse-bot**, follow these steps to install dependencies and configure the project.

### Software Requirements

- **C++17 or higher**: The project uses modern C++ features, so a compiler that supports C++17 is required.
- **CMake 3.10 or higher**: The project uses CMake for building and managing dependencies.
- **vcpkg**: The C++ package manager for managing project dependencies. You must have **vcpkg** installed and configured to work with the project.

### Dependencies

The following libraries are required to run the bot:

1. **fmt**: A modern C++ formatting library.
2. **ZLIB**: A compression library required for some of the dependencies.
3. **OpenSSL**: A toolkit for implementing secure communication protocols.
4. **CURL**: A library for transferring data with URLs, supporting a wide range of protocols (HTTP, HTTPS, FTP, etc.).
5. **nlohmann_json**: A header-only library for working with JSON in a modern and intuitive C++ style.
6. **Telegram Bot API**: The bot uses a static library for interacting with Telegram. This is included via the `libTgBot.a` file built by vcpkg.

These dependencies will be automatically installed via **vcpkg** during the setup process.

### Installation Steps

1. **Install CMake**: Ensure that you have CMake 3.10 or higher installed on your system. You can download it from [the official website](https://cmake.org/download/).

2. **Install a C++17 Compatible Compiler**.

3. **Clone the Project**:
   - Clone the repository:
     ```bash
     git clone https://github.com/yourusername/ticker-pulse-bot.git
     cd ticker-pulse-bot
     ```

4. **Install vcpkg**:
   - Windows:
     ```bash
     git clone https://github.com/microsoft/vcpkg.git vcpkg
     cd vcpkg
     .\bootstrap-vcpkg.bat
     ```
   - Linux / MacOS:
     ```bash
     git clone https://github.com/microsoft/vcpkg.git vcpkg
     cd vcpkg
     ./bootstrap-vcpkg.sh
     ```

5. **Install Dependencies with vcpkg**:
   - Install the necessary libraries via vcpkg:
     ```bash
     cd vcpkg
     ./vcpkg install fmt zlib openssl nlohmann-json tgbot-cpp libpq
     ```

6. **Prepare the config directory**:
   - It is necessary to create the "config" directory at the same level as src.
   - It should contain a configuration file global_config.h.
   - To work correctly, they must contain the variables VERSION, TELEGRAM_BOT_API_KEY, TELEGRAM_GROUP_ID, MAX_THREAD_COUNT, CHECK_CURRENCIES_LIMIT_INTERVAL, NEWS_API_KEY, PUBLISH_NEWS_INTERVAL.
   
7. **Configure the Project with CMake**:
   - Navigate to the project directory and configure the project:
     ```bash
     cd ticker-pulse-bot
     mkdir build
     cd build
     cmake ..
     ```

   - This will configure the project and prepare it for compilation.

9. **Build the Project**:
   - Run `make` to compile the bot:
     ```bash
     make
     ```

10. **Run the Bot**:
   - After successful compilation, you can run the bot using:
     ```bash
     ./TickerPulseBot
     ```

Make sure to follow the steps above to properly set up the environment and dependencies for the bot to function correctly.

## Architecture

The project is built using an object-oriented approach in C++. It uses the following components:

- **Telegram API**: For sending messages to a Telegram channel or chat.
- **Cryptocurrency Data API**: The bot uses an external service to get real-time cryptocurrency data.
- **Periodic Threshold Checking**: The bot regularly checks the current prices and alerts when events occur.
