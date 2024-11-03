// user.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "movie.h"
#include "movie_database.h"


class User {
private:
    std::string username;
    std::string password;
    std::vector<std::shared_ptr<Movie>> watchLater;
    std::vector<std::shared_ptr<Movie>> likedMovies;
    std::vector<std::string> searchHistory; // Nuevo: historial de búsquedas


public:
    User(const std::string& user, const std::string& pass);
    
    void addToWatchLater(std::shared_ptr<Movie> movie);
    void removeFromWatchLater(const std::string& movieId);
    void likeMovie(std::shared_ptr<Movie> movie);
    void unlikeMovie(const std::string& movieId);
    
    std::vector<std::shared_ptr<Movie>> getWatchLater() const;
    std::vector<std::shared_ptr<Movie>> getLikedMovies() const;
    std::string getUsername() const;
    bool verifyPassword(const std::string& pass) const;

    // Nuevos métodos
    void addSearchQuery(const std::string& query); // Nuevo
    const std::vector<std::string>& getSearchHistory() const; // Nuevo
    std::string serialize() const; // Nuevo
    static User deserialize(const std::string& data, const std::shared_ptr<MovieDatabase>& db); // Nuevo
};
