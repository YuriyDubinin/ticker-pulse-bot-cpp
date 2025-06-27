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

bool PostgresConnection::isConnected() const {
  return conn && PQstatus(conn) == CONNECTION_OK;
}

PGresult* PostgresConnection::executeQuery(const std::string& query) {
  if (!isConnected())
    return nullptr;
  PGresult* result = PQexec(conn, query.c_str());

  if (PQresultStatus(result) != PGRES_TUPLES_OK && PQresultStatus(result) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Query failed: {}\n", PQerrorMessage(conn));
    PQclear(result);
    return nullptr;
  }

  return result;
}

void PostgresConnection::clearResult(PGresult* result) {
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
