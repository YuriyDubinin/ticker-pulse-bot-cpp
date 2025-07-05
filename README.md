# ticker-pulse-bot-cpp v.1.0.4

### Installation Steps

1. **Install CMake**: Ensure that you have CMake 3.10 or higher installed on your system. You can download it from [the official website](https://cmake.org/download/).

2. **Install a C++17 Compatible Compiler**.

3. **Clone the Project**:
   - Clone the repository:
     ```bash
     git clone https://github.com/YuriyDubinin/ticker-pulse-bot.git
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
   - It should contain a configuration file global_config.h and global_config.cpp.
   - To work correctly, they must contain the necessary variables.
   
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