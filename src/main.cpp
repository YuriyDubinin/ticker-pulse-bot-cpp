#include "utils/Bot/Bot.h"

int main() {
    std::string version = "1.0.0";

    fmt::print("[TICKER-PULSE-BOT]: started, version: {}\n", version);

    Bot bot("7709079630:AAE06VwYO_l53Bns2Okn2QfCAjfJjEFRiAI", 2);

    bot.start();

    return 0;
}