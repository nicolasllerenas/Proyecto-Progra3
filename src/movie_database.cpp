#include "movie_database.h"
#include <fstream>
#include <sstream>
#include <future>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>

// Constructor
MovieDatabase::MovieDatabase() 
    : titleTrie(std::make_shared<Trie>()),
      plotTrie(std::make_shared<Trie>()),
      tagTrie(std::make_shared<Trie>()) {}

// Procesa una línea del archivo CSV, extrae los campos imdb_id, title, plot y tags, 
// crea un objeto Movie y lo inserta en las estructuras de datos correspondientes.
void MovieDatabase::processLine(const std::string& line) {
    std::stringstream ss(line);
    std::string imdb_id, title, plot, tags;
    
    std::getline(ss, imdb_id, ',');
    std::getline(ss, title, ',');
    std::getline(ss, plot, ',');
    std::getline(ss, tags, ',');
    
    // Process tags
    std::vector<std::string> tagList;
    std::stringstream tagStream(tags);
    std::string tag;
    while (std::getline(tagStream, tag, '|')) {
        tagList.push_back(tag);
    }
    
    auto movie = std::make_shared<Movie>(imdb_id, title, plot, tagList);
    
    std::lock_guard<std::mutex> lock(mtx);
    movies.push_back(movie);
    titleTrie->insert(title, movie);
    plotTrie->insert(plot, movie);
    for (const auto& tag : tagList) {
        tagTrie->insert(tag, movie);
    }
}

bool MovieDatabase::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    std::vector<std::future<void>> futures;
    while (std::getline(file, line)) {
        if (futures.size() >= std::thread::hardware_concurrency()) {
            // Espera a que se complete al menos un hilo antes de agregar otro
            futures.erase(std::remove_if(futures.begin(), futures.end(),
                [](std::future<void>& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }),
                futures.end());
        }
        // Lanzamos un nuevo hilo usando async en lugar de un thread manual
        futures.push_back(std::async(std::launch::async, &MovieDatabase::processLine, this, line));
    }

    // Espera a que todos los hilos terminen
    for (auto& f : futures) {
        f.get();
    }
    
    return true;
}

// Busca películas por título usando el Trie de títulos
std::vector<std::shared_ptr<Movie>> MovieDatabase::searchByTitle(const std::string& query) {
    return titleTrie->search(query);
}

// Busca películas por sinopsis usando el Trie de sinopsis
std::vector<std::shared_ptr<Movie>> MovieDatabase::searchByPlot(const std::string& query) {
    return plotTrie->search(query);
}

// Busca películas por etiquetas usando el Trie de etiquetas
std::vector<std::shared_ptr<Movie>> MovieDatabase::searchByTag(const std::string& tag) {
    return tagTrie->search(tag);
}

// Obtiene una película específica por su ID
std::shared_ptr<Movie> MovieDatabase::getMovieById(const std::string& id) {
    for (const auto& movie : movies) {
        if (movie->getId() == id) {
            return movie;
        }
    }
    return nullptr; // Si no se encuentra la película con el ID especificado
}