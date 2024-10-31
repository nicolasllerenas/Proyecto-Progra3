#include <iostream>
#include <memory>
#include "movie_database.h"
#include "search_engine.h"
#include "recommendation_system.h"
#include "authentication.h"
#include "user.h"

int main() {
    // Cargar la base de datos de películas
    auto database = std::make_shared<MovieDatabase>();
    if (!database->loadData("../data/movies.csv")) {
        std::cerr << "Error loading database\n";
        return 1;
    }

    auto searchEngine = std::make_shared<SearchEngine>(database);
    auto recommendationSystem = std::make_shared<RecommendationSystem>(database);
    Authentication auth;
    std::shared_ptr<User> currentUser = nullptr;

    // Menú de inicio
    std::cout << "Welcome to Progra3-Cinema!\n";
    while (!currentUser) {
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "Choose an option: ";
        int option;
        std::cin >> option;

        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (option == 1) {  // Login
            if (auth.loginUser(username, password)) {
                currentUser = std::make_shared<User>(username, password);
                std::cout << "Login successful.\n";
            } else {
                std::cout << "Invalid credentials.\n";
            }
        } else if (option == 2) {  // Register
            if (auth.registerUser(username, password)) {
                currentUser = std::make_shared<User>(username, password);
                std::cout << "Registration successful.\n";
            } else {
                std::cout << "Username already exists.\n";
            }
        }
    }

    // Menú principal
    while (true) {
        std::cout << "\nCurrent User: " << currentUser->getUsername() << "\n" 
                  << "Options:"
                  << "\n1. Search movies\n"
                  << "2. View watch later list\n"
                  << "3. View recommendations\n"
                  << "4. Exit\n"
                  << "Choose an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string query;
                std::cout << "Enter search term: ";
                std::cin.ignore();
                std::getline(std::cin, query);

                auto results = searchEngine->search(query);
                for (const auto& movie : results) {
                    std::cout << movie->getTitle() << "\n";
                }
                break;
            }
            case 2: {
                const auto& watchLater = currentUser->getWatchLater();
                for (const auto& movie : watchLater) {
                    std::cout << movie->getTitle() << "\n";
                }
                break;
            }
            case 3: {
                const auto& recommendations = recommendationSystem->getRecommendations(*currentUser);
                for (const auto& movie : recommendations) {
                    std::cout << movie->getTitle() << "\n";
                }
                break;
            }
            case 4:
                return 0;
        }
    }
    return 0;
}