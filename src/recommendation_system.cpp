// recommendation_system.cpp
#include "recommendation_system.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>

RecommendationSystem::RecommendationSystem(std::shared_ptr<MovieDatabase> db)
    : database(db) {}

double RecommendationSystem::calculateSimilarity(const Movie& movie1, const Movie& movie2) {
    auto tags1 = movie1.getTags();
    auto tags2 = movie2.getTags();
    
    // Crear conjuntos de tags para comparación rápida
    std::unordered_map<std::string, bool> tagSet;
    for (const auto& tag : tags1) {
        tagSet[tag] = true;
    }
    
    // Contar tags comunes
    int commonTags = 0;
    for (const auto& tag : tags2) {
        if (tagSet.find(tag) != tagSet.end()) {
            commonTags++;
        }
    }
    
    // Calcular similitud usando coeficiente de Jaccard
    double similarity = static_cast<double>(commonTags) / 
                       (tags1.size() + tags2.size() - commonTags);
                       
    return similarity;
}

std::vector<std::shared_ptr<Movie>> RecommendationSystem::getRecommendations(const User& user) {
    std::vector<std::shared_ptr<Movie>> recommendations;
    auto likedMovies = user.getLikedMovies();
    
    if (likedMovies.empty()) {
        // Si no hay películas con like, retornar lista vacía
        return recommendations;
    }
    
    // Mapa para almacenar puntuación de similitud de cada película
    std::unordered_map<std::string, double> movieScores;
    std::unordered_map<std::string, bool> likedMoviesSet;
    
    // Crear conjunto de películas con like para búsqueda rápida
    for (const auto& movie : likedMovies) {
        likedMoviesSet[movie->getId()] = true;
    }
    
    // Para cada película que le gustó al usuario
    for (const auto& likedMovie : likedMovies) {
        // Buscar películas con tags similares
        for (const auto& tag : likedMovie->getTags()) {
            auto similarMovies = database->searchByTag(tag);
            
            // Calcular puntuación para cada película similar
            for (const auto& candidate : similarMovies) {
                // Saltar si la película ya tiene like
                if (likedMoviesSet.find(candidate->getId()) != likedMoviesSet.end()) {
                    continue;
                }
                
                // Calcular similitud y actualizar puntuación
                double similarity = calculateSimilarity(*likedMovie, *candidate);
                movieScores[candidate->getId()] += similarity;
            }
        }
    }
    
    // Convertir el mapa de puntuaciones a vector para ordenar
    std::vector<std::pair<std::string, double>> scores;
    for (const auto& [movieId, score] : movieScores) {
        scores.push_back({movieId, score});
    }
    
    // Ordenar por puntuación
    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Tomar las top 10 recomendaciones
    for (int i = 0; i < std::min(10, static_cast<int>(scores.size())); i++) {
        if (auto movie = database->getMovieById(scores[i].first)) {
            recommendations.push_back(movie);
        }
    }
    
    return recommendations;
}