#include "bot.h"
#include "global_config.h"

int main() {
    fmt::print("[TICKER-PULSE-BOT]: started, version: {}\n", VERSION);

    Bot bot(API_KEY, 2);

    bot.start();

    return 0;
};