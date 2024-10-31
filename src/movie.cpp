// movie.cpp
#include "movie.h"
#include <algorithm>

Movie::Movie(const std::string& id, const std::string& t, 
             const std::string& plot, const std::vector<std::string>& movie_tags)
    : imdb_id(id), title(t), plot_synopsis(plot), tags(movie_tags), importance_score(0) {}

std::string Movie::getId() const { return imdb_id; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getPlot() const { return plot_synopsis; }
std::vector<std::string> Movie::getTags() const { return tags; }
int Movie::getImportanceScore() const { return importance_score; }

void Movie::setImportanceScore(int score) {
    importance_score = score;
}

void Movie::calculateImportance(const std::string& searchTerm) {
    int score = 0;
    
    // Búsqueda en título (mayor peso)
    if (title.find(searchTerm) != std::string::npos) {
        score += 10;
    }
    
    // Búsqueda en sinopsis
    if (plot_synopsis.find(searchTerm) != std::string::npos) {
        score += 5;
    }
    
    // Búsqueda en tags
    for (const auto& tag : tags) {
        if (tag.find(searchTerm) != std::string::npos) {
            score += 3;
        }
    }
    
    importance_score = score;
}
