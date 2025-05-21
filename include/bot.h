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
        Bot(const std::string& token, int threads_count);
        void start();

    private:
        static std::map<std::string, std::string> crypto_map;
        TgBot::Bot bot;
        ThreadPool pool;
        CryptoFetcher fetcher;
        std::map<std::string, std::vector<double>> limites;
        TgBot::InlineKeyboardMarkup::Ptr create_main_keyboard();
        void on_start_command(TgBot::Message::Ptr message);
        void on_any_message(TgBot::Message::Ptr message);
        void send_to_group(const std::string& group_id, const std::string& message_text);
        void on_callback_query(TgBot::CallbackQuery::Ptr callback_query);
        void set_currency_limites();
        void check_limit_values_at_interval(const unsigned int seconds);
};

#endif
