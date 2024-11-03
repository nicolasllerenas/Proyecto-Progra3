#include "authentication.h"
#include <fstream>
#include <iostream>
#include <filesystem>


Authentication::Authentication(std::shared_ptr<MovieDatabase> db) 
    : filename("../data/users.txt"), database(db) {
    std::filesystem::create_directories("../data/users");
    loadUsers();
}

// Cargar usuarios desde archivo en el mapa
void Authentication::loadUsers() {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string username, password;
        while (file >> username >> password) {
            users[username] = password;
        }
        file.close();
    }
}

// Guardar un usuario en el archivo
void Authentication::saveUser(const std::string& username, const std::string& password) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << username << " " << password << "\n";
        file.close();
    }
}

// Registrar un nuevo usuario
bool Authentication::registerUser(const std::string& username, const std::string& password) {
    if (users.find(username) == users.end()) {
        users[username] = password;
        saveUser(username, password);
        return true;
    }
    return false;
}

// Verificar credenciales
bool Authentication::verifyCredentials(const std::string& username, const std::string& password) const {
    auto it = users.find(username);
    return it != users.end() && it->second == password;
}

// Cargar datos del usuario
std::shared_ptr<User> Authentication::loadUserData(const std::string& username, const std::string& password) {
    if (!verifyCredentials(username, password)) {
        return nullptr;
    }

    // Intentar cargar datos del usuario
    std::string dataFilename = getUserDataFilename(username);
    std::ifstream file(dataFilename);
    if (file.is_open()) {
        std::string data;
        std::getline(file, data);
        file.close();
        return std::make_shared<User>(User::deserialize(data, database));
    }
    
    // Si no hay datos, crear nuevo usuario
    return std::make_shared<User>(username, password);
}

// Guardar datos del usuario
void Authentication::saveUserData(const User& user) {
    std::string dataFilename = getUserDataFilename(user.getUsername());
    std::ofstream file(dataFilename);
    if (file.is_open()) {
        file << user.serialize();
        file.close();
    }
}

std::string Authentication::getUserDataFilename(const std::string& username) const {
    return "../data/users/" + username + "_data.txt";  // Guardamos en el directorio de datos
}