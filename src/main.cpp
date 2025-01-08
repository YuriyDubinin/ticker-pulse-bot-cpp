#include <iostream>
#include <fmt/core.h>
#include <tgbot/tgbot.h>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <functional>
#include "utils/ThreadPool/ThreadPool.h" 

int main() {
    std::string version = "1.0.0";

    fmt::print("[TICKER-PULSE-BOT]: started, version: {}\n", version);

    // API ключ
    TgBot::Bot bot("");

    // Создание пула потоков с 2 потоками
    ThreadPool pool(2);

    // Обработка команды /start
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        // Создание кнопок
        TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
        button1->text = "📉 Курс валют";
        button1->callbackData = "ACTUAL";

        // TgBot::InlineKeyboardButton::Ptr button2(new TgBot::InlineKeyboardButton);
        // button2->text = "⬆️ Выбрать";
        // button2->callbackData = "SELECT_CURRENCY";

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
    });

    // Обработка любых сообщений
    bot.getEvents().onAnyMessage([&bot, &pool](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }

        // Добавляем задачу в пул потоков
        pool.enqueueTask([&bot, message]() {
            bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        });
    });

    // Обработка callback-данных
    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr callbackQuery) {
        if (callbackQuery->data == "ACTUAL") {
            bot.getApi().sendMessage(callbackQuery->message->chat->id, "Список актуального курса топовых криптовалют"); // Пример ответа
        } else if (callbackQuery->data == "SELECT_CURRENCY") {
            bot.getApi().sendMessage(callbackQuery->message->chat->id, "Вызов меню выбора криптовалюты"); // Пример ответа
        } else if (callbackQuery->data == "ATIONS") {
            bot.getApi().sendMessage(callbackQuery->message->chat->id, "Список команд бота");
        } else if (callbackQuery->data == "INFO") {
            bot.getApi().sendMessage(callbackQuery->message->chat->id, "Обновления: Бот теперь поддерживает новые валюты.");
        }
    });

    try {
        // Печать имени бота
        printf("[TICKER-PULSE-BOT]: TG username - %s\n", bot.getApi().getMe()->username.c_str());

        // Запуск Long Polling
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("[TICKER-PULSE-BOT]: Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("[TICKER-PULSE-BOT]: %s\n", e.what());
    }

    return 0;
}
