#pragma once
#include <string>
#include <unordered_map>

class Authentication {
private:
    std::unordered_map<std::string, std::string> users;  // Mapa de usuario: contraseña
    const std::string filename = "../data/usuarios.txt";          // Nombre del archivo de persistencia

    void loadUsers();                                     // Cargar usuarios desde archivo
    void saveUser(const std::string& username, const std::string& password); // Guardar usuario en archivo

public:
    Authentication();                                     // Constructor que carga usuarios
    bool registerUser(const std::string& username, const std::string& password); // Registrar usuario
    bool loginUser(const std::string& username, const std::string& password) const; // Iniciar sesión
};