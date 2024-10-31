// user.cpp
#include "user.h"
#include <algorithm>

User::User(const std::string& user, const std::string& pass) 
    : username(user), password(pass) {}

void User::addToWatchLater(std::shared_ptr<Movie> movie) {
    // Verificar si la película ya está en la lista
    auto it = std::find_if(watchLater.begin(), watchLater.end(),
        [&movie](const auto& m) { return m->getId() == movie->getId(); });
    
    if (it == watchLater.end()) {
        watchLater.push_back(movie);
    }
}

void User::removeFromWatchLater(const std::string& movieId) {
    watchLater.erase(
        std::remove_if(watchLater.begin(), watchLater.end(),
            [&movieId](const auto& movie) { return movie->getId() == movieId; }),
        watchLater.end()
    );
}

void User::likeMovie(std::shared_ptr<Movie> movie) {
    // Verificar si la película ya está en likes
    auto it = std::find_if(likedMovies.begin(), likedMovies.end(),
        [&movie](const auto& m) { return m->getId() == movie->getId(); });
    
    if (it == likedMovies.end()) {
        likedMovies.push_back(movie);
    }
}

void User::unlikeMovie(const std::string& movieId) {
    likedMovies.erase(
        std::remove_if(likedMovies.begin(), likedMovies.end(),
            [&movieId](const auto& movie) { return movie->getId() == movieId; }),
        likedMovies.end()
    );
}

std::vector<std::shared_ptr<Movie>> User::getWatchLater() const {
    return watchLater;
}

std::vector<std::shared_ptr<Movie>> User::getLikedMovies() const {
    return likedMovies;
}

std::string User::getUsername() const {
    return username;
}

bool User::verifyPassword(const std::string& pass) const {
    return password == pass;
}