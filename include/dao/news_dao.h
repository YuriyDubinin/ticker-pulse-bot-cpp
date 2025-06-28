#ifndef NEWS_DAO_H
#define NEWS_DAO_H

#include <optional>
#include <vector>
#include "news_model.h"
#include "postgres_connection.h"

class NewsDAO {
  public:
    explicit NewsDAO(PostgresConnection& db);

    bool insert(const News& news);
    bool update(const News& news);
    bool remove(int id);

    std::optional<News> get_by_id(int id);
    std::optional<News> get_latest_unpublished();

  private:
    PostgresConnection& db_;

    News parse_news_from_result(PGresult* result, int row);
    bool prepare_statements();
};

#endif
