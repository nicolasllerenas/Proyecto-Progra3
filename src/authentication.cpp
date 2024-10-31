#include "authentication.h"
#include <fstream>
#include <iostream>

// Constructor que carga usuarios desde el archivo al iniciar el programa
Authentication::Authentication() {
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

// Iniciar sesión validando usuario y contraseña
bool Authentication::loginUser(const std::string& username, const std::string& password) const {
    auto it = users.find(username);
    return it != users.end() && it->second == password;
}