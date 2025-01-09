#include <string>
#include <fmt/core.h>
#include "bot.h"
#include "global_config.h"
#include "bot.h"
#include "crypto_fetcher.h"

Bot::Bot(const std::string& token, int threadCount)
    : bot(token), pool(threadCount) {};

void Bot::start() {
    // Обработка команды /start
    bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
        onStartCommand(message);
    });

    // Обработка любых сообщений
    bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
        onAnyMessage(message);
    });

    // Обработка callback-данных
    bot.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr callbackQuery) {
        onCallbackQuery(callbackQuery);
    });

    try {
        // Печать имени бота
        fmt::print("[TICKER_PULSE_BOT]: TG username - {}\n", bot.getApi().getMe()->username);

        // Запуск Long Polling
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            fmt::print("[TICKER_PULSE_BOT]: Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        fmt::print("[TICKER_PULSE_BOT]: {}\n", e.what());
    }
};

void Bot::onStartCommand(TgBot::Message::Ptr message) {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

    // Создание кнопок
    TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
    button1->text = "📉 Курс валют";
    button1->callbackData = "ACTUAL";

    TgBot::InlineKeyboardButton::Ptr button3(new TgBot::InlineKeyboardButton);
    button3->text = "⚙️ Команды";
    button3->callbackData = "ATIONS";

    TgBot::InlineKeyboardButton::Ptr button4(new TgBot::InlineKeyboardButton);
    button4->text = "ℹ️ Информация";
    button4->callbackData = "INFO";

    // Добавление кнопок в ряды
    keyboard->inlineKeyboard.push_back({button1});
    keyboard->inlineKeyboard.push_back({button3, button4});

    // Отправка приветственного сообщения с клавиатурой
    bot.getApi().sendMessage(message->chat->id, "📊 Привет! Я отслеживаю курсы криптовалют. 🚀", false, 0, keyboard);
};

void Bot::onAnyMessage(TgBot::Message::Ptr message) {
    fmt::print("User wrote {}\n", message->text);

    if (message->text.find("/start") != 0) {
        // Добавляем задачу в пул потоков
        pool.enqueueTask([this, message]() {
            bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        });
    }
};

void Bot::sendToGroup(const std::string& groupId, const std::string& messageText) {
    try {
        bot.getApi().sendMessage(groupId, messageText);
        fmt::print("[TICKER_PULSE_BOT]: Message sent to group {}\n", groupId);
    } catch (TgBot::TgException& e) {
        fmt::print("[TICKER_PULSE_BOT]: Failed to send message to group {}: {}\n", groupId, e.what());
    }
};

void Bot::onCallbackQuery(TgBot::CallbackQuery::Ptr callbackQuery) {
    if (callbackQuery->data == "ACTUAL") { 
        CryptoFetcher fetcher;
        const std::string symbol = "bitcoin";
        nlohmann::json result = fetcher.fetchCoinGecko(symbol);

        if (!result.empty()) {
            try {
                fmt::print("JSON объект: {}\n", result.dump(4));
            } catch (const nlohmann::json::exception& e) {
                fmt::print(stderr, "[TICKER_PULSE_BOT]: Error when retrieving data from JSON: {}\n", e.what());
            }
        } else {
            fmt::print(stderr, "[TICKER_PULSE_BOT]: Error while receiving data\n");
        }

        bot.getApi().sendMessage(callbackQuery->message->chat->id, "Список актуального курса топовых криптовалют: \n");
    } else if (callbackQuery->data == "SELECT_CURRENCY") {
        bot.getApi().sendMessage(callbackQuery->message->chat->id, "Вызов меню выбора криптовалюты");
    } else if (callbackQuery->data == "ATIONS") {
        bot.getApi().sendMessage(callbackQuery->message->chat->id, "Список команд бота");
    } else if (callbackQuery->data == "INFO") {
        bot.getApi().sendMessage(callbackQuery->message->chat->id, "Обновления: Бот теперь поддерживает новые валюты.");
    }
};
