#include "news_dao.h"
#include <libpq-fe.h>
#include <string>

NewsDAO::NewsDAO(PostgresConnection& db) : db_(db) {
  prepare_statements();
}

bool NewsDAO::prepare_statements() {
  bool ok = true;
  ok &= db_.prepare_statement(
      "insert_news",
      "INSERT INTO news (uid, title, content, source, source_logo_id, source_url, story_path, published, link) "
      "VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9);");

  ok &= db_.prepare_statement("update_news",
                              "UPDATE news SET uid=$1, title=$2, content=$3, source=$4, source_logo_id=$5, "
                              "source_url=$6, story_path=$7, published=$8, link=$9, updated_at=CURRENT_TIMESTAMP "
                              "WHERE id=$10;");

  ok &= db_.prepare_statement("delete_news", "DELETE FROM news WHERE id=$1;");

  ok &= db_.prepare_statement("select_news_by_id",
                              "SELECT id, uid, title, content, source, source_logo_id, source_url, story_path, "
                              "published, link, created_at, updated_at "
                              "FROM news WHERE id=$1 LIMIT 1;");

  ok &= db_.prepare_statement("select_latest_news",
                              "SELECT id, uid, title, content, source, source_logo_id, source_url, story_path, "
                              "published, link, created_at, updated_at "
                              "FROM news ORDER BY created_at DESC LIMIT 1;");

  ok &= db_.prepare_statement("select_all_news",
                              "SELECT id, uid, title, content, source, source_logo_id, source_url, story_path, "
                              "published, link, created_at, updated_at "
                              "FROM news ORDER BY created_at DESC;");

  ok &= db_.prepare_statement("select_latest_unpublished_news",
                              "SELECT id, uid, title, content, source, source_logo_id, source_url, story_path, "
                              "published, link, created_at, updated_at "
                              "FROM news WHERE published = false ORDER BY id DESC LIMIT 1;");

  return ok;
}

News NewsDAO::parse_news_from_result(PGresult* res, int row) {
  News news;
  news.id             = std::stoi(PQgetvalue(res, row, 0));
  news.uid            = PQgetvalue(res, row, 1);
  news.title          = PQgetvalue(res, row, 2);
  news.content        = PQgetisnull(res, row, 3) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 3));
  news.source         = PQgetisnull(res, row, 4) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 4));
  news.source_logo_id = PQgetisnull(res, row, 5) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 5));
  news.source_url     = PQgetisnull(res, row, 6) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 6));
  news.story_path     = PQgetisnull(res, row, 7) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 7));

  if (PQgetisnull(res, row, 8)) {
    news.published = std::nullopt;
  } else {
    news.published = (std::string(PQgetvalue(res, row, 8)) == "t");
  }

  news.link       = PQgetisnull(res, row, 9) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 9));
  news.created_at = PQgetisnull(res, row, 10) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 10));
  news.updated_at = PQgetisnull(res, row, 11) ? std::nullopt : std::make_optional(PQgetvalue(res, row, 11));
  return news;
}

bool NewsDAO::insert(const News& news) {
  const char* params[9] = {news.uid.c_str(),
                           news.title.c_str(),
                           news.content ? news.content->c_str() : nullptr,
                           news.source ? news.source->c_str() : nullptr,
                           news.source_logo_id ? news.source_logo_id->c_str() : nullptr,
                           news.source_url ? news.source_url->c_str() : nullptr,
                           news.story_path ? news.story_path->c_str() : nullptr,
                           news.published ? (*news.published ? "t" : "f") : nullptr,
                           news.link ? news.link->c_str() : nullptr};

  PGresult* res = db_.execute_prepared_statement("insert_news", 9, params);
  if (!res)
    return false;

  bool ok = PQresultStatus(res) == PGRES_COMMAND_OK;
  PQclear(res);
  return ok;
}

bool NewsDAO::update(const News& news) {
  std::string idStr      = std::to_string(news.id);
  const char* params[10] = {news.uid.c_str(),
                            news.title.c_str(),
                            news.content ? news.content->c_str() : nullptr,
                            news.source ? news.source->c_str() : nullptr,
                            news.source_logo_id ? news.source_logo_id->c_str() : nullptr,
                            news.source_url ? news.source_url->c_str() : nullptr,
                            news.story_path ? news.story_path->c_str() : nullptr,
                            news.published ? (*news.published ? "t" : "f") : nullptr,
                            news.link ? news.link->c_str() : nullptr,
                            idStr.c_str()};

  PGresult* res = db_.execute_prepared_statement("update_news", 10, params);
  if (!res)
    return false;

  bool ok = PQresultStatus(res) == PGRES_COMMAND_OK;
  PQclear(res);
  return ok;
}

bool NewsDAO::remove(int id) {
  std::string idStr     = std::to_string(id);
  const char* params[1] = {idStr.c_str()};

  PGresult* res = db_.execute_prepared_statement("delete_news", 1, params);
  if (!res)
    return false;

  bool ok = PQresultStatus(res) == PGRES_COMMAND_OK;
  PQclear(res);
  return ok;
}

std::optional<News> NewsDAO::get_by_id(int id) {
  std::string idStr     = std::to_string(id);
  const char* params[1] = {idStr.c_str()};
  PGresult*   res       = db_.execute_prepared_statement("select_news_by_id", 1, params);
  if (!res || PQntuples(res) == 0) {
    if (res)
      PQclear(res);
    return std::nullopt;
  }
  News news = parse_news_from_result(res, 0);
  PQclear(res);
  return news;
}

std::optional<News> NewsDAO::get_latest_unpublished() {
  PGresult* res = db_.execute_prepared_statement("select_latest_unpublished_news", 0, nullptr);
  if (!res || PQntuples(res) == 0) {
    if (res)
      PQclear(res);
    return std::nullopt;
  }
  News news = parse_news_from_result(res, 0);
  PQclear(res);
  return news;
}