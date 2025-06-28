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

    bool      is_connected() const;
    PGresult* execute_query(const std::string& query);
    void      clear_result(PGresult* result);
    void      close();

    PGconn* raw(); // Доступ к PGconn* при необходимости

    bool      prepare_statement(const std::string& stmtName, const std::string& query);
    PGresult* execute_prepared_statement(const std::string& stmtName, int nParams, const char* const* paramValues);

  private:
    PGconn* conn;
};

#endif
