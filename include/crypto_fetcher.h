#ifndef CRYPTO_FETCHER_H
#define CRYPTO_FETCHER_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class CryptoFetcher {
    public:
        explicit CryptoFetcher();
        ~CryptoFetcher();

        nlohmann::json fetch_coingecko(const std::string& url);
    
    private:
        static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif