#include "postgres_connection.h"
#include <fmt/core.h>

PostgresConnection::PostgresConnection(const std::string& connection_info) {
  conn = PQconnectdb(connection_info.c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Failed to connect to PostgreSQL: {}\n", PQerrorMessage(conn));
    PQfinish(conn);
    conn = nullptr;
  }
}

PostgresConnection::~PostgresConnection() {
  close();
}

bool PostgresConnection::is_connected() const {
  return conn && PQstatus(conn) == CONNECTION_OK;
}

PGresult* PostgresConnection::execute_query(const std::string& query) {
  if (!is_connected())
    return nullptr;
  PGresult* result = PQexec(conn, query.c_str());

  if (PQresultStatus(result) != PGRES_TUPLES_OK && PQresultStatus(result) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Query failed: {}\n", PQerrorMessage(conn));
    PQclear(result);
    return nullptr;
  }

  return result;
}

void PostgresConnection::clear_result(PGresult* result) {
  if (result) {
    PQclear(result);
  }
}

void PostgresConnection::close() {
  if (conn) {
    PQfinish(conn);
    conn = nullptr;
  }
}

PGconn* PostgresConnection::raw() {
  return conn;
}

// Подготовить запрос
bool PostgresConnection::prepare_statement(const std::string& stmtName, const std::string& query) {
  if (!is_connected())
    return false;
  PGresult* res = PQprepare(conn, stmtName.c_str(), query.c_str(), 0, nullptr);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Prepare failed: {}\n", PQerrorMessage(conn));
    PQclear(res);
    return false;
  }
  PQclear(res);
  return true;
}

// Выполнить подготовленный запрос
PGresult* PostgresConnection::execute_prepared_statement(const std::string& stmtName,
                                                         int                nParams,
                                                         const char* const* paramValues) {
  if (!is_connected())
    return nullptr;
  PGresult* res = PQexecPrepared(conn, stmtName.c_str(), nParams, paramValues, nullptr, nullptr, 0);
  if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: ExecPrepared failed: {}\n", PQerrorMessage(conn));
    PQclear(res);
    return nullptr;
  }
  return res;
}
