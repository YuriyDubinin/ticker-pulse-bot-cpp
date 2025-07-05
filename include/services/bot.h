#ifndef BOT_H
#define BOT_H

#include <fmt/core.h>
#include <map>
#include <string>
#include <tgbot/tgbot.h>
#include <vector>
#include "http_client.h"
#include "news_dao.h"
#include "postgres_connection.h"
#include "thread_pool.h"

class Bot {
  public:
    Bot(const std::string& token, int threads_count, const std::string& db_connection_info);
    void start();

  private:
    TgBot::Bot                       bot;
    ThreadPool                       pool;
    HTTPClient                       http_client;
    PostgresConnection               db;
    NewsDAO                          newsDAO;
    TgBot::InlineKeyboardMarkup::Ptr create_main_keyboard();
    void                             on_start_command(TgBot::Message::Ptr message);
    void                             on_any_message(TgBot::Message::Ptr message);
    void send_message_to_group(const std::string& group_id, const std::string& message_text);
    void publish_news_by_interval(const unsigned int minutes);
};

#endif
