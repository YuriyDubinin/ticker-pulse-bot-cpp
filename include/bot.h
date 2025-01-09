#ifndef BOT_H
#define BOT_H

#include <tgbot/tgbot.h>
#include <fmt/core.h>
#include "thread_pool.h"

class Bot {
    public:
        Bot(const std::string& token, int threadCount);
        void start();
        void onStartCommand(TgBot::Message::Ptr message);
        void onAnyMessage(TgBot::Message::Ptr message);
        void sendToGroup(const std::string& groupId, const std::string& messageText);
        void onCallbackQuery(TgBot::CallbackQuery::Ptr callbackQuery);

    private:
        TgBot::Bot bot;
        ThreadPool pool;
};

#endif
