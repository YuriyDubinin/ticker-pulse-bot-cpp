#ifndef BOT_H
#define BOT_H

#include <fmt/core.h>
#include <map>
#include <string>
#include <tgbot/tgbot.h>
#include <vector>
#include "http_client.h"
#include "thread_pool.h"

class Bot {
  public:
    Bot(const std::string& token, int threads_count);
    void start();

  private:
    TgBot::Bot                                 bot;
    ThreadPool                                 pool;
    HTTPClient                                 http_client;
    std::map<std::string, std::vector<double>> limites;
    TgBot::InlineKeyboardMarkup::Ptr           create_main_keyboard();
    void                                       on_start_command(TgBot::Message::Ptr message);
    void                                       on_any_message(TgBot::Message::Ptr message);
    void send_message_to_group(const std::string& group_id, const std::string& message_text);
    void publish_news_by_interval(const unsigned int seconts);
};

#endif
