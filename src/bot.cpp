#include <string>
#include <vector>
#include <fmt/core.h>
#include <chrono>
#include "bot.h"
#include "global_config.h"
#include "bot.h"
#include "crypto_fetcher.h"
#include "utils.h"

Bot::Bot(const std::string& token, int threadCount)
    : bot(token), pool(threadCount) {};

void Bot::start() {
    // Обработка команды /start
    bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
        onStartCommand(message);
    });

    // Обработка команды /help
    bot.getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
       bot.getApi().sendMessage(message->chat->id, 
        "📔 Список команд бота:\n\n" 
        "/start - Начать общение с ботом\n" 
        "/help - Получить справку по командам\n" 
        "/info - Получить информацию о боте"
        );
    });

    // Обработка команды /info
    bot.getEvents().onCommand("info", [this](TgBot::Message::Ptr message) {
       bot.getApi().sendMessage(message->chat->id, 
        "ℹ️ Бот предназначен для предоставления информации о криптовалюте, на данный моент находится на стадии разработки."
        );
    });

    // Обработка остальных сообщений
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
    TgBot::InlineKeyboardMarkup::Ptr mainKeyboard = createMainKeyboard();
    bot.getApi().sendMessage(message->chat->id, "📊 Привет! Я отслеживаю курсы криптовалют. 🚀", false, 0, mainKeyboard);
};

void Bot::onAnyMessage(TgBot::Message::Ptr message) {
    fmt::print("User wrote {}\n", message->text);

    if (
        message->text.find("/start") != 0 &&
        message->text.find("/help") != 0 &&
        message->text.find("/info") != 0
    ) {
        // Добавляем задачу в пул потоков
        pool.enqueueTask([this, message]() {
            bot.getApi().sendMessage(message->chat->id, "Не понимаю что делать с " + message->text + ".\n📔 Список команд бота:\n\n" 
            "/start - Начать общение с ботом\n" 
            "/help - Получить справку по командам\n" 
            "/info - Получить информацию о боте");
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
        std::map<std::string, std::string> cryptoMap = {
            {"bitcoin", "BTC"},
            {"ethereum", "ETH"},
            {"tether", "USDT"},
            {"binancecoin", "BNB"},
            {"usd-coin", "USDC"},
            {"ripple", "XRP"},
            {"cardano", "ADA"},
            {"dogecoin", "DOGE"},
            {"solana", "SOL"},
        };
        std::vector<std::string> cryptoKeysVector;  // for request to CoinGecko

        for (const auto& pair : cryptoMap) {
            cryptoKeysVector.push_back(pair.first);
        };

        std::string symbolsString = utils::stringifyStringsVectorToString(cryptoKeysVector, ",");
        std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + symbolsString + "&vs_currencies=usd";
        nlohmann::json result = fetcher.fetchCoinGecko(url);
        std::string answer = "📈 Актуальный курс\nведущих криптовалют: \n";

        if (!result.empty()) {
            try {
                for (const auto& [key, value] : result.items()) {
                    double usdValue = value.at("usd");
                    answer = answer + "\n" + utils::capitalizeFirstLetter(key) + " " + cryptoMap[key] + ": " + utils::toFixedDouble(usdValue, 2) + " $";
                };

                bot.getApi().sendMessage(callbackQuery->message->chat->id, answer);
            } catch (const nlohmann::json::exception& e) {
                fmt::print(stderr, "[TICKER_PULSE_BOT]: Error when retrieving data from JSON: {}\n", e.what());
            }
        } else {
            fmt::print(stderr, "[TICKER_PULSE_BOT]: Error while receiving data\n");
        }
    } else if (callbackQuery->data == "SELECT_CURRENCY") {
        bot.getApi().sendMessage(callbackQuery->message->chat->id, "Вызов меню выбора криптовалюты");
    } else if (callbackQuery->data == "ATIONS") {
        bot.getApi().sendMessage(callbackQuery->message->chat->id, 
            "📔 Список команд бота:\n\n" 
            "/start - Начать общение с ботом\n" 
            "/help - Получить справку по командам\n" 
            "/info - Получить информацию о боте"
        );
    } else if (callbackQuery->data == "INFO") {
        bot.getApi().sendMessage(callbackQuery->message->chat->id, 
            "ℹ️ Бот предназначен для предоставления информации о криптовалюте, на данный моент находится на стадии разработки."
        );
    }
};

TgBot::InlineKeyboardMarkup::Ptr Bot::createMainKeyboard() {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

    // Создание кнопок
    TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
    button1->text = "📈 Курс ведущих валют";
    button1->callbackData = "ACTUAL";

    TgBot::InlineKeyboardButton::Ptr button3(new TgBot::InlineKeyboardButton);
    button3->text = "📔 Команды";
    button3->callbackData = "ATIONS";

    TgBot::InlineKeyboardButton::Ptr button4(new TgBot::InlineKeyboardButton);
    button4->text = "ℹ️ Информация";
    button4->callbackData = "INFO";

    // Добавление кнопок в ряды
    keyboard->inlineKeyboard.push_back({button1});
    keyboard->inlineKeyboard.push_back({button3, button4});

    return keyboard;
};