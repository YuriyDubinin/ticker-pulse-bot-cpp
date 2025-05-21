#ifndef NEWS_FETCHER
#define NEWS_FETCHER

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class NewsFetcher {
    public:
        explicit NewsFetcher();
        ~NewsFetcher();

        nlohmann::json fetch_coingecko(const std::string& url);
};

#endif