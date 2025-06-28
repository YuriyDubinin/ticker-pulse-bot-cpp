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

    bool prepare_statement(const std::string& statement_name, const std::string& query);
    PGresult*
    execute_prepared_statement(const std::string& statement_name, int params_count, const char* const* param_values);

  private:
    PGconn* conn;
};

#endif
