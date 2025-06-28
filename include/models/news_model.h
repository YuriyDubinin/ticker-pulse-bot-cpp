#ifndef NEWS_H
#define NEWS_H

#include <optional>
#include <string>

struct News {
    int                        id;
    std::string                uid;
    std::string                title;
    std::optional<std::string> content;
    std::optional<std::string> source;
    std::optional<std::string> source_logo_id;
    std::optional<std::string> source_url;
    std::optional<std::string> story_path;
    std::optional<bool>        published;
    std::optional<std::string> link;
    std::optional<std::string> created_at;
    std::optional<std::string> updated_at;

    News();
    News(int                               id_,
         const std::string&                uid_,
         const std::string&                title_,
         const std::optional<std::string>& content_,
         const std::optional<std::string>& source_,
         const std::optional<std::string>& source_logo_id_,
         const std::optional<std::string>& source_url_,
         const std::optional<std::string>& story_path_,
         const std::optional<bool>&        published_,
         const std::optional<std::string>& link_,
         const std::optional<std::string>& created_at_,
         const std::optional<std::string>& updated_at_);
};

#endif
