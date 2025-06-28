#ifndef NEWS_DAO_H
#define NEWS_DAO_H

#include <optional>
#include <vector>
#include "news_model.h"
#include "postgres_connection.h"

class NewsDAO {
  public:
    explicit NewsDAO(PostgresConnection& db);

    std::optional<News> getById(int id);
    std::optional<News> getLatestNews();
    std::vector<News>   getAllNews();

    bool insert(const News& news);
    bool update(const News& news);
    bool remove(int id);

  private:
    PostgresConnection& db_;

    News parseNewsFromResult(PGresult* result, int row);
    bool prepareStatements();
};

#endif
