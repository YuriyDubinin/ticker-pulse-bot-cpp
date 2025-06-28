#include "news_model.h"

News::News() = default;

News::News(int                               id_,
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
           const std::optional<std::string>& updated_at_)
    : id(id_), uid(uid_), title(title_), content(content_), source(source_), source_logo_id(source_logo_id_),
      source_url(source_url_), story_path(story_path_), published(published_), link(link_), created_at(created_at_),
      updated_at(updated_at_) {
}
