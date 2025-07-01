#include "bot.h"
#include "global_config.h"
#include "postgres_connection.h"

int main() {
  fmt::print("[TICKER_PULSE_BOT]: started, version: {}\n", VERSION);

  Bot bot(TELEGRAM_BOT_API_KEY, MAX_THREAD_COUNT, DB_CONNECTION_INFO);

  bot.start();

  return 0;
};