#include "bot.h"
#include "global_config.h"

int main() {
    fmt::print("[TICKER_PULSE_BOT]: started, version: {}\n", VERSION);

    Bot bot(TELEGRAM_BOT_API_KEY, 4);

    bot.start();

    return 0;
};