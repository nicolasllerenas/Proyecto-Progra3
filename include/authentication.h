#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "user.h"
#include "movie_database.h"

class Authentication {
private:
    std::string filename;
    std::unordered_map<std::string, std::string> users;
    std::shared_ptr<MovieDatabase> database;

public:
    explicit Authentication(std::shared_ptr<MovieDatabase> db = nullptr);
    
    void loadUsers();
    void saveUser(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
    bool verifyCredentials(const std::string& username, const std::string& password) const;
    std::shared_ptr<User> loadUserData(const std::string& username, const std::string& password);
    void saveUserData(const User& user);

private:
    std::string getUserDataFilename(const std::string& username) const;
};