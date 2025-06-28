#include "bot.h"
#include <chrono>
#include <fmt/core.h>
#include <string>
#include <thread>
#include <vector>
#include "common_utils.h"
#include "global_config.h"
#include "http_client.h"
#include "thread_pool.h"

Bot::Bot(const std::string& token, int threads_count) : bot(token), pool(threads_count), http_client() {};

std::map<std::string, std::string> Bot::crypto_map = {
    {"bitcoin", "Bitcoin (BTC)"},          {"ethereum", "Etherium (ETH)"},  {"tether", "Tether ERC-20 (USDT)"},
    {"binancecoin", "Binance Coin (BNB)"}, {"usd-coin", "Usd Coin (USDC)"}, {"ripple", "Ripple (XRP)"},
    {"cardano", "Cardano (ADA)"},          {"dogecoin", "Dogecoin (DOGE)"}, {"solana", "Solana (SOL)"},
    {"the-open-network", "Toncoin (TON)"},
};

void Bot::start() {
  // Обработка команды /start
  // bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
  //     on_start_command(message);
  // });

  // Обработка команды /help
  // bot.getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
  //    bot.getApi().sendMessage(message->chat->id,
  //     "📔 Список команд бота:\n\n"
  //     "/start - Начать общение с ботом\n"
  //     "/help - Получить справку по командам\n"
  //     "/info - Получить информацию о боте"
  //     );
  // });

  // Обработка команды /info
  // bot.getEvents().onCommand("info", [this](TgBot::Message::Ptr message) {
  //    bot.getApi().sendMessage(message->chat->id,
  //     "ℹ️ Бот предназначен для предоставления информации о криптовалюте, на данный моент находится на стадии
  //     разработки."
  //     );
  // });

  // Обработка остальных сообщений
  // bot.getEvents().on_any_message([this](TgBot::Message::Ptr message) {
  //     // on_any_message(message);
  // });

  // Обработка callback-данных
  // bot.getEvents().on_callback_query([this](TgBot::CallbackQuery::Ptr callback_query) {
  //     on_callback_query(callback_query);
  // });

  try {
    const std::string start_chat_message = "🚀 Ticker Pulse Bot запущен..";
    send_to_group(TELEGRAM_GROUP_ID, start_chat_message);

    fmt::print("[TICKER_PULSE_BOT]: TG username - {}\n", bot.getApi().getMe()->username);

    pool.enqueue_task([this]() {
      set_currency_limites();
    });

    pool.enqueue_task([this]() {
      check_limit_values_by_interval(CHECK_CURRENCIES_LIMIT_INTERVAL);
    });

    pool.enqueue_task([this]() {
      publish_news_by_interval(PUBLISH_NEWS_INTERVAL);
    });

    // Запуск long polling
    TgBot::TgLongPoll longPoll(bot);
    fmt::print("[TICKER_PULSE_BOT]: Long poll started\n");

    while (true) {
      // fmt::print("[TICKER_PULSE_BOT]: Long poll started\n");
      longPoll.start();
    }
  } catch (TgBot::TgException& e) {
    fmt::print("[TICKER_PULSE_BOT]: {}\n", e.what());
  };
};

void Bot::on_start_command(TgBot::Message::Ptr message) {
  TgBot::InlineKeyboardMarkup::Ptr main_keyboard = create_main_keyboard();
  bot.getApi().sendMessage(message->chat->id, "📊 Привет! Я отслеживаю курсы криптовалют. 🚀", false, 0, main_keyboard);
};

void Bot::on_any_message(TgBot::Message::Ptr message) {
  fmt::print("[TICKER_PULSE_BOT]: [CHAT]: {}\n", message->text);

  if (message->text.find("/start") != 0 && message->text.find("/help") != 0 && message->text.find("/info") != 0) {
    bot.getApi().sendMessage(message->chat->id,
                             "Не понимаю что делать с " + message->text +
                                 ".\n📔 Список команд бота:\n\n"
                                 "/start - Начать общение с ботом\n"
                                 "/help - Получить справку по командам\n"
                                 "/info - Получить информацию о боте");
  }
};

void Bot::send_to_group(const std::string& group_id, const std::string& message_text) {
  try {
    bot.getApi().sendMessage(group_id, message_text);
    fmt::print("[TICKER_PULSE_BOT]: Message sent to group {}\n", group_id);
  } catch (TgBot::TgException& e) {
    fmt::print("[TICKER_PULSE_BOT]: Failed to send message to group {}: {}\n", group_id, e.what());
  };
};

void Bot::on_callback_query(TgBot::CallbackQuery::Ptr callback_query) {
  if (callback_query->data == "ACTUAL") {
    std::vector<std::string> crypto_keys_vector; // для запросов к CoinGecko

    for (const auto& pair : crypto_map) {
      crypto_keys_vector.push_back(pair.first);
    };

    std::string currencies_string = common_utils::stringify_strings_vector_to_string(crypto_keys_vector, ",");
    std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + currencies_string + "&vs_currencies=usd";
    nlohmann::json result = http_client.fetch_json(url);
    std::string    answer = "📈 Актуальный курс\nведущих криптовалют: \n";

    if (!result.empty()) {
      try {
        for (const auto& [key, value] : result.items()) {
          double usdValue = value.at("usd");
          answer = answer + fmt::format("\n {}: {} $", crypto_map[key], common_utils::to_fixed_double(usdValue, 2));
        };

        bot.getApi().sendMessage(callback_query->message->chat->id, answer);
      } catch (const nlohmann::json::exception& e) {
        bot.getApi().sendMessage(callback_query->message->chat->id, "⌛ Я загружен, скоро освобожусь.");
        fmt::print(stderr, "[TICKER_PULSE_BOT]: Error when retrieving data from JSON: {}\n", e.what());
      }
    } else {
      fmt::print(stderr, "[TICKER_PULSE_BOT]: Error while receiving data\n");
    }
  } else if (callback_query->data == "SELECT_CURRENCY") {
    bot.getApi().sendMessage(callback_query->message->chat->id, "Вызов меню выбора криптовалюты");
  } else if (callback_query->data == "ATIONS") {
    bot.getApi().sendMessage(callback_query->message->chat->id,
                             "📔 Список команд бота:\n\n"
                             "/start - Начать общение с ботом\n"
                             "/help - Получить справку по командам\n"
                             "/info - Получить информацию о боте");
  } else if (callback_query->data == "INFO") {
    bot.getApi().sendMessage(callback_query->message->chat->id,
                             "ℹ️ Бот предназначен для предоставления информации о криптовалюте, на данный моент "
                             "находится на стадии разработки.");
  }
};

TgBot::InlineKeyboardMarkup::Ptr Bot::create_main_keyboard() {
  TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

  // Создание кнопок
  TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
  button1->text         = "📈 Курс ведущих валют";
  button1->callbackData = "ACTUAL";

  // TgBot::InlineKeyboardButton::Ptr button3(new TgBot::InlineKeyboardButton);
  // button3->text = "📔 Команды";
  // button3->callbackData = "ATIONS";

  // TgBot::InlineKeyboardButton::Ptr button4(new TgBot::InlineKeyboardButton);
  // button4->text = "ℹ️ Информация";
  // button4->callbackData = "INFO";

  // Добавление кнопок в ряды
  keyboard->inlineKeyboard.push_back({button1});
  // keyboard->inlineKeyboard.push_back({button3, button4});

  return keyboard;
};

void Bot::set_currency_limites() {
  fmt::print("[TICKER_PULSE_BOT]: set_currency_limites started\n");

  const std::map<std::string, std::vector<double>> updated_limites;
  const unsigned int                               interval = 20;

  for (const auto& [currency_name, symbol] : crypto_map) {
    try {
      std::string url =
          fmt::format("https://api.coingecko.com/api/v3/coins/{}/market_chart?vs_currency=usd&days=7", currency_name);
      nlohmann::json result = http_client.fetch_json(url);

      std::vector<double> min_max_values = common_utils::find_currency_min_max(result["prices"]);
      limites[currency_name]             = min_max_values;

      fmt::print("[TICKER_PULSE_BOT]: {}, min: {}, max: {}\n", currency_name, min_max_values[0], min_max_values[1]);
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    } catch (const std::exception& e) {
      fmt::print("[TICKER_PULSE_BOT]: [set_currency_limites]: Error setting limites: {}\n", e.what());
    };
  }
};

void Bot::check_limit_values_by_interval(const unsigned int seconds) {
  std::this_thread::sleep_for(
      std::chrono::seconds(seconds));          // первый sleep нужен чтобы подождать всех высчитывавений лимитов
  std::vector<std::string> crypto_keys_vector; // для запросов к CoinGecko

  for (const auto& pair : crypto_map) {
    crypto_keys_vector.push_back(pair.first);
  };

  std::string currencies_string = common_utils::stringify_strings_vector_to_string(crypto_keys_vector, ",");
  std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + currencies_string + "&vs_currencies=usd";

  while (true) {
    nlohmann::json result = http_client.fetch_json(url);

    if (!result.empty()) {
      try {
        for (const auto& [key, value] : result.items()) {
          if (limites.find(key) != limites.end()) {
            std::string   message;
            double        usdValue = value.at("usd");
            const double& min      = limites[key][0];
            const double& max      = limites[key][1];

            if (usdValue < min) {
              message = fmt::format("⬇️ {} {}: {} $, это ниже минимальной цены за последние 7 дней! ({} $)", key,
                                    crypto_map[key], common_utils::to_fixed_double(usdValue, 2),
                                    common_utils::to_fixed_double(min, 2));
              send_to_group(TELEGRAM_GROUP_ID, message);
            }

            if (usdValue > max) {
              message = fmt::format("⬆️ {} {}: {} $, это выше максимальной цены за последние 7 дней! ({} $)", key,
                                    crypto_map[key], common_utils::to_fixed_double(usdValue, 2),
                                    common_utils::to_fixed_double(max, 2));
              send_to_group(TELEGRAM_GROUP_ID, message);
            }
          }
        };
      } catch (const std::exception& e) {
        fmt::print("[TICKER_PULSE_BOT]: [check_limit_values_by_interval]: Error sending message to group: {}\n",
                   e.what());
      };
    }

    std::this_thread::sleep_for(std::chrono::seconds(seconds));
  }
};

void Bot::publish_news_by_interval(const unsigned int seconds) {
  std::string url = "https://newsdata.io/api/1/latest?apikey=" + NEWS_API_KEY + "&q=криптовалюта&language=ru&size=1";

  while (true) {
    nlohmann::json result = http_client.fetch_json(url);

    try {
      nlohmann::json article = result["results"][0];

      std::string title       = article.value("title", "Без заголовка");
      std::string description = article.value("description", "Нет описания");
      std::string link        = article.value("link", "Нет ссылки");
      std::string message     = fmt::format("{}.\n{} @ticker_pulse\n\n🔗{}", title, description, link);

      send_to_group(TELEGRAM_GROUP_ID, message);
    } catch (const std::exception& e) {
      fmt::print("[TICKER_PULSE_BOT]: [publish_news_by_interval]: Error sending message to group: {}\n", e.what());
    }

    std::this_thread::sleep_for(std::chrono::seconds(seconds));
  }
}
