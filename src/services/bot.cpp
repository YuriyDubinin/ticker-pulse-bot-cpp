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

Bot::Bot(const std::string& token, int threads_count, const std::string& db_connection_info)
    : bot(token), pool(threads_count), http_client(), db(db_connection_info), newsDAO(db) {};

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
    // const std::string start_chat_message = "🚀 Ticker Pulse Bot запущен..";
    // send_message_to_group(TELEGRAM_GROUP_ID, start_chat_message);

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

void Bot::publish_news_by_interval(const unsigned int minutes) {
  while (true) {
    auto news_opt = newsDAO.get_latest_unpublished();

    if (news_opt.has_value()) {
      News        news         = news_opt.value();
      std::string message_text = fmt::format("{}. @ticker_pulse\n🔗 Подробнее: {}", news.title, news.link.value_or(""));

      send_message_to_group(TELEGRAM_GROUP_ID, message_text);

      news.published = true;

      if (newsDAO.update(news)) {
        // fmt::print("[TICKER_PULSE_BOT]: News with id {} marked as published.\n", news.id);
      } else {
        // fmt::print("[TICKER_PULSE_BOT]: Failed to update news with id {}.\n", news.id);
      }
    }

    std::this_thread::sleep_for(std::chrono::minutes(minutes));
  }
}
