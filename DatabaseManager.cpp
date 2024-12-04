#include "DatabaseManager.h"

void DatabaseManager::handleCSV(){
    const std::string inputFile = "mpst_full_data.csv";
    const std::string outputFile = "mpst_full_data_modified.csv";
    processCSV(inputFile, outputFile);
    std::vector<MovieData> movies = readCSV(outputFile);
    for (const auto& movie : movies) {
        movieSearchTree.insert(movie);
    }
}

// Menú para interactuar con el user
void DatabaseManager::runMenu() {
    int choice = 0;
    while (true)
    {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Borrar el búfer
        handleMenuChoice(choice);
    }
}

void DatabaseManager::displayMenu() {
    std::cout << "\n===== Menú de películas =====\n";
    std::cout << "1. Buscar en título o sinopsis\n";
    std::cout << "2. Buscar por Tags\n";
    std::cout << "3. Ver películas con a las que diste Me Gusta\n";
    std::cout << "4. Ver lista de Ver Más Tarde\n";
    std::cout << "5. Salir\n";
    std::cout << "Ingrese una opción: ";
}

void DatabaseManager::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        searchMoviesByTitleOrPlot();
        break;
    case 2:
        searchMoviesByTags();
        break;
    case 3:
        viewLikedMovies();
        break;
    case 4:
        viewWatchLaterMovies();
        break;
    case 5:
        std::cout << "\nAdiós!\n";
        exit(0);
        break;
    default:
        std::cout << "\nOpción no valida, intente de nuevo\n";
        break;
    }
}

void DatabaseManager::searchMoviesByTitleOrPlot() {
    std::cout << "\nIngrese un término para buscar: ";
    std::string term;
    std::getline(std::cin, term);

    std::vector<MovieData> results = movieSearchTree.search(term);

    if (results.empty()) {
        std::cout << "\nNo se encontraron películas.\n";
    } else {
        std::cout << "\nResultados de búsqueda:\n";
        printMovies(results);

        handleSearchResultSelection(results);
    }
}

void DatabaseManager::searchMoviesByTags() {
    std::cout << "\nIngrese un tag para buscar: ";
    std::string term;
    std::getline(std::cin, term);

    std::vector<MovieData> results = movieSearchTree.searchByTags(term);

    if (results.empty()) {
        std::cout << "\nNo se encontraron películas.\n";
    } else {
        std::cout << "\nResultados de búsqueda:\n";
        printMovies(results);

        handleSearchResultSelection(results);
    }
}

void DatabaseManager::handleSearchResultSelection(const std::vector<MovieData>& results) {
    std::cout << "\nEntre el número de la película que quiera guardar (0 para ignorar): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Limpiar el búfer

    if (choice > 0 && choice <= static_cast<int>(results.size())) {
        const MovieData& selectedMovie = results[choice - 1];

        std::cout << "1. Añadir a Me Gusta\n";
        std::cout << "2. Añadir a Ver Más Tarde\n";
        std::cout << "Ingresa tu elección: ";
        int listChoice;
        std::cin >> listChoice;
        std::cin.ignore(); // limpiar el búfer

        if (listChoice == 1) {
            likedMovies.push_back(selectedMovie);
            std::cout << "\nAñadido a Me Gusta.\n";
        } else if (listChoice == 2) {
            watchLaterMovies.push_back(selectedMovie);
            std::cout << "\nAñadido a Ver Más Tarde.\n";
        } else {
            std::cout << "\nOpción no válida, regresando...\n";
        }
    } else if (choice == 0) {
        std::cout << "\nNo se elegió una película, regresando...\n";
    } else {
        std::cout << "\nOpción no válida, regresando...\n";
    }
}

void DatabaseManager::viewLikedMovies() {
    if (likedMovies.empty()) {
        std::cout << "\nLista de Me Gusta vacía...\n";
    } else {
        std::cout << "\nPelículas que me gustan:\n";
        for (const auto& movie : likedMovies) {
            std::cout<<movie;
        }
    }
}

void DatabaseManager::viewWatchLaterMovies() {
    if (watchLaterMovies.empty()) {
        std::cout << "\nLista Ver Más Tarde vacía....\n";
    } else {
        std::cout << "\nVer Más Tarde:\n";
        for (const auto& movie : watchLaterMovies) {
            std::cout<<movie;
        }
    }
}