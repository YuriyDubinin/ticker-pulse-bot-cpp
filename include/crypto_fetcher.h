#ifndef CRYPTO_FETCHER
#define CRYPTO_FETCHER

#include <string>
#include <memory>
#include <thread>
#include <future>
#include "thread_pool.h"

class CryptoFetcher {
    public:
        explicit CryptoFetcher(ThreadPool& pool) : pool_(pool) {};
        ~CryptoFetcher() = default;

        void fetchCoinGecko(const std::string& symbol);
    private:
        ThreadPool& pool_;
};

#endif