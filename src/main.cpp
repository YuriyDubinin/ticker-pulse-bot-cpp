#include "bot.h"
#include "global_config.h"

int main() {
    fmt::print("[TICKER_PULSE_BOT]: started, version: {}\n", VERSION);

    Bot bot(API_KEY, 2);

    bot.start();

    return 0;
};