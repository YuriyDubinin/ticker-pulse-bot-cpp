#ifndef NEWS_FETCHER_H
#define NEWS_FETCHER_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class NewsFetcher {
    public:
        explicit NewsFetcher();
        ~NewsFetcher();

        nlohmann::json fetch_news(const std::string& url);
    
    private:
        static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif