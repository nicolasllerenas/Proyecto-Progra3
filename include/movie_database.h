// movie_database.h
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "movie.h"
#include "trie.h"

class MovieDatabase {
private:
    std::vector<std::shared_ptr<Movie>> movies;
    std::shared_ptr<Trie> titleTrie;
    std::shared_ptr<Trie> plotTrie;
    std::shared_ptr<Trie> tagTrie;
    void processLine(const std::string& line);
    std::mutex mtx;  // Mutex para la sincronización de hilos

public:
    MovieDatabase();
    bool loadData(const std::string& filename);
    std::vector<std::shared_ptr<Movie>> searchByTitle(const std::string& query);
    std::vector<std::shared_ptr<Movie>> searchByPlot(const std::string& query);
    std::vector<std::shared_ptr<Movie>> searchByTag(const std::string& tag);
    std::shared_ptr<Movie> getMovieById(const std::string& id);
};

