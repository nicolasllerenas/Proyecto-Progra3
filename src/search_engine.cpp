// search_engine.cpp
#include "search_engine.h"
#include <algorithm>
#include <thread>
#include <mutex>

SearchEngine::SearchEngine(std::shared_ptr<MovieDatabase> db) 
    : database(db) {}

std::vector<std::shared_ptr<Movie>> SearchEngine::search(const std::string& query, int page) {
    // Resultados de búsqueda combinados
    std::vector<std::shared_ptr<Movie>> combinedResults;
    std::mutex resultsMutex;

    // Búsqueda concurrente en título, plot y tags
    std::thread titleSearch([&]() {
        auto results = database->searchByTitle(query);
        std::lock_guard<std::mutex> lock(resultsMutex);
        combinedResults.insert(combinedResults.end(), results.begin(), results.end());
    });

    std::thread plotSearch([&]() {
        auto results = database->searchByPlot(query);
        std::lock_guard<std::mutex> lock(resultsMutex);
        combinedResults.insert(combinedResults.end(), results.begin(), results.end());
    });

    std::thread tagSearch([&]() {
        auto results = database->searchByTag(query);
        std::lock_guard<std::mutex> lock(resultsMutex);
        combinedResults.insert(combinedResults.end(), results.begin(), results.end());
    });

    titleSearch.join();
    plotSearch.join();
    tagSearch.join();

    // Eliminar duplicados
    std::sort(combinedResults.begin(), combinedResults.end(),
        [](const auto& a, const auto& b) { return a->getId() < b->getId(); });
    combinedResults.erase(
        std::unique(combinedResults.begin(), combinedResults.end(),
            [](const auto& a, const auto& b) { return a->getId() == b->getId(); }),
        combinedResults.end()
    );

    // Calcular importancia para cada película
    for (auto& movie : combinedResults) {
        movie->calculateImportance(query);
    }

    // Ordenar por importancia
    std::sort(combinedResults.begin(), combinedResults.end(),
        [](const auto& a, const auto& b) { 
            return a->getImportanceScore() > b->getImportanceScore(); 
        });

    // Obtener página específica de resultados
    int startIdx = page * RESULTS_PER_PAGE;
    int endIdx = std::min(startIdx + RESULTS_PER_PAGE, static_cast<int>(combinedResults.size()));

    if (startIdx >= combinedResults.size()) {
        return {};
    }

    return std::vector<std::shared_ptr<Movie>>(
        combinedResults.begin() + startIdx,
        combinedResults.begin() + endIdx
    );
}

std::vector<std::shared_ptr<Movie>> SearchEngine::searchByTag(const std::string& tag, int page) {
    auto results = database->searchByTag(tag);
    
    // Ordenar por importancia
    std::sort(results.begin(), results.end(),
        [](const auto& a, const auto& b) { 
            return a->getImportanceScore() > b->getImportanceScore(); 
        });

    // Obtener página específica de resultados
    int startIdx = page * RESULTS_PER_PAGE;
    int endIdx = std::min(startIdx + RESULTS_PER_PAGE, static_cast<int>(results.size()));

    if (startIdx >= results.size()) {
        return {};
    }

    return std::vector<std::shared_ptr<Movie>>(
        results.begin() + startIdx,
        results.begin() + endIdx
    );
}
