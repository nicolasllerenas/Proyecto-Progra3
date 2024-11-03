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

// Nuevos métodos
void User::addSearchQuery(const std::string& query) {
    searchHistory.push_back(query);
}

const std::vector<std::string>& User::getSearchHistory() const {
    return searchHistory;
}

std::string User::serialize() const {
    std::stringstream ss;
    
    // Formato: username|password|movieId1,movieId2|likedId1,likedId2|search1,search2
    ss << username << "|" << password << "|";
    
    // Serializar Watch Later
    for (const auto& movie : watchLater) {
        ss << movie->getId() << ",";
    }
    ss << "|";
    
    // Serializar Liked Movies
    for (const auto& movie : likedMovies) {
        ss << movie->getId() << ",";
    }
    ss << "|";
    
    // Serializar Search History
    for (const auto& search : searchHistory) {
        ss << search << ",";
    }
    
    return ss.str();
}

User User::deserialize(const std::string& data, const std::shared_ptr<MovieDatabase>& db) {
    std::stringstream ss(data);
    std::string username, password, watchLaterStr, likedStr, searchStr;
    
    std::getline(ss, username, '|');
    std::getline(ss, password, '|');
    std::getline(ss, watchLaterStr, '|');
    std::getline(ss, likedStr, '|');
    std::getline(ss, searchStr, '|');
    
    User user(username, password);
    
    // Parsear Watch Later
    std::stringstream watchStream(watchLaterStr);
    std::string movieId;
    while (std::getline(watchStream, movieId, ',')) {
        if (!movieId.empty()) {
            if (auto movie = db->getMovieById(movieId)) {
                user.addToWatchLater(movie);
            }
        }
    }
    
    // Parsear Liked Movies
    std::stringstream likedStream(likedStr);
    while (std::getline(likedStream, movieId, ',')) {
        if (!movieId.empty()) {
            if (auto movie = db->getMovieById(movieId)) {
                user.likeMovie(movie);
            }
        }
    }
    
    // Parsear Search History
    std::stringstream searchStream(searchStr);
    std::string search;
    while (std::getline(searchStream, search, ',')) {
        if (!search.empty()) {
            user.addSearchQuery(search);
        }
    }
    
    return user;
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