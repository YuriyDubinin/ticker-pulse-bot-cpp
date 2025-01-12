#ifndef BOT_H
#define BOT_H

#include <tgbot/tgbot.h>
#include <fmt/core.h>
#include <map>
#include <string>
#include <vector>
#include "thread_pool.h"
#include "crypto_fetcher.h"


class Bot {
    public:
        Bot(const std::string& token, int threadCount);
        void start();

    private:
        static std::map<std::string, std::string> cryptoMap;
        TgBot::Bot bot;
        ThreadPool pool;
        CryptoFetcher fetcher;
        std::map<std::string, std::vector<double>> limites;
        TgBot::InlineKeyboardMarkup::Ptr createMainKeyboard();
        void onStartCommand(TgBot::Message::Ptr message);
        void onAnyMessage(TgBot::Message::Ptr message);
        void sendToGroup(const std::string& groupId, const std::string& messageText);
        void onCallbackQuery(TgBot::CallbackQuery::Ptr callbackQuery);
        void setCurrencyLimites();
        void checkLimitValuesAtInterval();
};

#endif
