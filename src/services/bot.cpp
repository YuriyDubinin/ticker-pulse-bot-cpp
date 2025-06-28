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

  try {
    const std::string start_chat_message = "🚀 Ticker Pulse Bot запущен..";
    send_message_to_group(TELEGRAM_GROUP_ID, start_chat_message);

    fmt::print("[TICKER_PULSE_BOT]: TG username - {}\n", bot.getApi().getMe()->username);

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

void Bot::send_message_to_group(const std::string& group_id, const std::string& message_text) {
  try {
    bot.getApi().sendMessage(group_id, message_text);
    fmt::print("[TICKER_PULSE_BOT]: Message sent to group {}\n", group_id);
  } catch (TgBot::TgException& e) {
    fmt::print("[TICKER_PULSE_BOT]: Failed to send message to group {}: {}\n", group_id, e.what());
  };
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

      send_message_to_group(TELEGRAM_GROUP_ID, message);
    } catch (const std::exception& e) {
      fmt::print("[TICKER_PULSE_BOT]: [publish_news_by_interval]: Error sending message to group: {}\n", e.what());
    }

    std::this_thread::sleep_for(std::chrono::seconds(seconds));
  }
}
