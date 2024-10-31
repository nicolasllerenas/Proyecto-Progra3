// user.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "movie.h"

class User {
private:
    std::string username;
    std::string password;
    std::vector<std::shared_ptr<Movie>> watchLater;
    std::vector<std::shared_ptr<Movie>> likedMovies;

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
};
