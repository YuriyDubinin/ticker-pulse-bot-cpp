#include "utils/Bot/Bot.h"
#include "../configs/config.h"

int main() {
    fmt::print("[TICKER-PULSE-BOT]: started, version: {}\n", VERSION);

    Bot bot(API_KEY, 2);

    bot.start();

    return 0;
};