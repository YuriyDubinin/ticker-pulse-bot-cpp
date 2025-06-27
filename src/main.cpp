#include "bot.h"
#include "global_config.h"
#include "postgres_connection.h"

int main() {
    fmt::print("[TICKER_PULSE_BOT]: started, version: {}\n", VERSION);

    Bot bot(TELEGRAM_BOT_API_KEY, MAX_THREAD_COUNT);
    PostgresConnection db(DB_CONNECTION_INFO);

    PGresult* result = db.executeQuery("SELECT * FROM news ORDER BY created_at DESC LIMIT 1");

    if (!result) {
        fmt::print("[MAIN]: Ошибка при выполнении запроса к news.\n");
        return 1;
    }

    int rows = PQntuples(result);
    if (rows == 0) {
        fmt::print("[MAIN]: В таблице news нет записей.\n");
        db.clearResult(result);
        return 0;
    }

    std::string id        = PQgetvalue(result, 0, PQfnumber(result, "id"));
    std::string title     = PQgetvalue(result, 0, PQfnumber(result, "title"));


    fmt::print("\n[ПОСЛЕДНЯЯ НОВОСТЬ]:\nID: {}\nЗаголовок: {}\n\n", id, title);

    // bot.start();

    return 0;
};