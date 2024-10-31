// search_engine.h
#pragma once
#include <memory>
#include <vector>
#include "movie_database.h"

class SearchEngine {
private:
    std::shared_ptr<MovieDatabase> database;
    static const int RESULTS_PER_PAGE = 5;

public:
    SearchEngine(std::shared_ptr<MovieDatabase> db);
    std::vector<std::shared_ptr<Movie>> search(const std::string& query, int page = 0);
    std::vector<std::shared_ptr<Movie>> searchByTag(const std::string& tag, int page = 0);
};
