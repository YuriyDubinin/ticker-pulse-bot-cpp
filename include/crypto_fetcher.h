#ifndef CRYPTO_FETCHER
#define CRYPTO_FETCHER

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class CryptoFetcher {
    public:
        explicit CryptoFetcher();
        ~CryptoFetcher();

        nlohmann::json fetchCoinGecko(const std::string& symbol);
    
private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif