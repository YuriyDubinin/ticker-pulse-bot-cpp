#ifndef POSTGRESS_CONNECTION
#define POSTGRESS_CONNECTION

#include <libpq-fe.h>
#include <string>

class PostgresConnection {
  public:
    explicit PostgresConnection(const std::string& connection_info);
    ~PostgresConnection();

    PostgresConnection(const PostgresConnection&)            = delete;
    PostgresConnection& operator=(const PostgresConnection&) = delete;

    bool      isConnected() const;
    PGresult* executeQuery(const std::string& query);
    void      clearResult(PGresult* result);
    void      close();

    PGconn* raw(); // Доступ к PGconn* при необходимости

  private:
    PGconn* conn;
};

#endif
