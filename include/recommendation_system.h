// recommendation_system.h
#pragma once
#include <memory>
#include <vector>
#include "user.h"
#include "movie_database.h"

class RecommendationSystem {
private:
    std::shared_ptr<MovieDatabase> database;

public:
    RecommendationSystem(std::shared_ptr<MovieDatabase> db);
    std::vector<std::shared_ptr<Movie>> getRecommendations(const User& user);
    double calculateSimilarity(const Movie& movie1, const Movie& movie2);
};