#include "movie_database.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <vector>

// Constructor
MovieDatabase::MovieDatabase()
    : titleTrie(std::make_shared<Trie>()),
      plotTrie(std::make_shared<Trie>()),
      tagTrie(std::make_shared<Trie>()),
      done(false) {} // Inicializa done en false

// Destructor para asegurarse de que los hilos se detienen limpiamente
MovieDatabase::~MovieDatabase() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        done = true;
    }
    queueCondVar.notify_all(); // Notificar a todos los hilos de trabajo para que terminen
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// Procesa una línea del archivo CSV y crea un objeto Movie
void MovieDatabase::processLine(const std::string& line) {
    std::stringstream ss(line);
    std::string imdb_id, title, plot, tags;
    
    std::getline(ss, imdb_id, ',');
    std::getline(ss, title, ',');
    std::getline(ss, plot, ',');
    std::getline(ss, tags, ',');

    // Procesar etiquetas
    std::vector<std::string> tagList;
    std::stringstream tagStream(tags);
    std::string tag;
    while (std::getline(tagStream, tag, '|')) {
        tagList.push_back(tag);
    }
    
    auto movie = std::make_shared<Movie>(imdb_id, title, plot, tagList);
    
    // Bloqueo para inserciones en estructuras de datos compartidas
    std::lock_guard<std::mutex> lock(mtx);
    movies.push_back(movie);
    titleTrie->insert(title, movie);
    plotTrie->insert(plot, movie);
    for (const auto& tag : tagList) {
        tagTrie->insert(tag, movie);
    }
}

// Función que ejecutará cada hilo de trabajo para procesar líneas de la cola
void MovieDatabase::workerThread() {
    while (true) {
        std::string line;
        
        // Bloqueo para acceder a la cola
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondVar.wait(lock, [this] { return !taskQueue.empty() || done; });
            
            if (done && taskQueue.empty()) {
                return; // Salir si no hay más tareas y la carga de datos ha terminado
            }
            
            line = taskQueue.front();
            taskQueue.pop();
        }

        // Procesa la línea fuera del bloqueo de la cola
        processLine(line);
    }
}

// Cargar datos desde un archivo CSV y lanzar hilos de trabajo
bool MovieDatabase::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::getline(file, line); // Omite la cabecera

    // Lanzar un grupo fijo de hilos de trabajo
    const unsigned int numThreads = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < numThreads; ++i) {
        workers.emplace_back(&MovieDatabase::workerThread, this);
    }

    // Leer el archivo y añadir cada línea a la cola de tareas
    while (std::getline(file, line)) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(line);
        }
        queueCondVar.notify_one(); // Notificar a un hilo para que procese la línea
    }

    // Indicar que se ha terminado de leer el archivo
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        done = true;
    }
    queueCondVar.notify_all(); // Notificar a todos los hilos de trabajo

    return true;
}

// Métodos de búsqueda usando los tries
std::vector<std::shared_ptr<Movie>> MovieDatabase::searchByTitle(const std::string& query) {
    return titleTrie->search(query);
}

std::vector<std::shared_ptr<Movie>> MovieDatabase::searchByPlot(const std::string& query) {
    return plotTrie->search(query);
}

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