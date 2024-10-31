#pragma once
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "movie.h"
#include "trie.h"

class MovieDatabase {
public:
    MovieDatabase();
    ~MovieDatabase(); // Destructor para detener los hilos de trabajo

    // Carga de datos desde un archivo CSV
    bool loadData(const std::string& filename);

    // Métodos de búsqueda
    std::vector<std::shared_ptr<Movie>> searchByTitle(const std::string& query);
    std::vector<std::shared_ptr<Movie>> searchByPlot(const std::string& query);
    std::vector<std::shared_ptr<Movie>> searchByTag(const std::string& tag);
    std::shared_ptr<Movie> getMovieById(const std::string& id);

private:
    // Procesamiento de línea y función de trabajo de hilo
    void processLine(const std::string& line);
    void workerThread(); // Función para cada hilo de trabajo

    // Tries para búsquedas
    std::shared_ptr<Trie> titleTrie;
    std::shared_ptr<Trie> plotTrie;
    std::shared_ptr<Trie> tagTrie;

    // Almacén de películas y tareas
    std::vector<std::shared_ptr<Movie>> movies;
    std::queue<std::string> taskQueue;

    // Variables de sincronización
    std::vector<std::thread> workers; // Vector de hilos de trabajo
    std::mutex queueMutex; // Mutex para proteger la cola
    std::mutex mtx; // Mutex para proteger las estructuras de datos compartidas
    std::condition_variable queueCondVar; // Para notificar a los hilos de trabajo
    bool done = false; // Indica cuando se ha completado la carga de tareas
};