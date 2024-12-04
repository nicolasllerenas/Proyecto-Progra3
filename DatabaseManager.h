#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "treeHandle.h"
#include "csvHandle.h"

// (Singleton)
class DatabaseManager {
private:
    static DatabaseManager* instance;
    DatabaseManager() {}  // constructor privado para que no se pueda instanciar
    DatabaseManager(const DatabaseManager&) = delete;  // para que no se pueda copiar
    DatabaseManager& operator=(const DatabaseManager&) = delete;  // para que no se pueda asignar

    MovieSearchTree movieSearchTree;  // Un solo tree

    std::vector<MovieData> likedMovies; //peliculas con like
    std::vector<MovieData> watchLaterMovies; //peliculas ver más tarde
public:
    static DatabaseManager* getInstance() {
        if (instance == nullptr) {
            instance = new DatabaseManager();
        }
        return instance;
    }

    ~DatabaseManager(){};

    void handleCSV();
    void runMenu();
    void displayMenu();
    void handleMenuChoice(int);
    void searchMoviesByTitleOrPlot();
    void searchMoviesByTags();
    void handleSearchResultSelection(const std::vector<MovieData>&);
    void viewLikedMovies();
    void viewWatchLaterMovies();
};


#endif