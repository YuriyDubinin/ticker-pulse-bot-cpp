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
    TgBot::Bot bot("API_KEY");

    // Создание пула потоков с 2 потоками
    ThreadPool pool(2);

    // Обработка команды /start
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "📊 Привет! Я отслеживаю курсы криптовалют. 🚀");
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