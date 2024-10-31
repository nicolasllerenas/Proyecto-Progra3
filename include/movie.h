// movie.h
#pragma once
#include <string>
#include <vector>

class Movie {
private:
    std::string imdb_id;
    std::string title;
    std::string plot_synopsis;
    std::vector<std::string> tags;
    int importance_score;

public:
    Movie(const std::string& id, const std::string& t, 
          const std::string& plot, const std::vector<std::string>& movie_tags);
    
    // Getters
    std::string getId() const;
    std::string getTitle() const;
    std::string getPlot() const;
    std::vector<std::string> getTags() const;
    int getImportanceScore() const;
    
    // Setters
    void setImportanceScore(int score);
    
    // Métodos para calcular importancia
    void calculateImportance(const std::string& searchTerm);
};