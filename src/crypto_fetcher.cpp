#include <string>
#include "crypto_fetcher.h"

void CryptoFetcher::fetchCoinGecko(const std::string& symbol) {
    pool_.enqueueTask([this, symbol]() {
        
    });
};