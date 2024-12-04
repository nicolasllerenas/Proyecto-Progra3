#ifndef MOVIEDATA_H
#define MOVIEDATA_H

#include <string>
#include <iostream>

//el struct cricial determinante
struct MovieData {
    std::string imdb_id;
    std::string title;
    std::string plot_synopsis;
    std::string tags;
    std::string split;
    std::string synopsis_source;

    // sobrecarga para imprimir más fácil
    friend std::ostream& operator<<(std::ostream& os, const MovieData& movie) {
        os << "IMDB ID: " << movie.imdb_id << "\n"
           << "Title: " << movie.title << "\n"
           << "Plot Synopsis: " << movie.plot_synopsis << "\n"
           << "Tags: " << movie.tags << "\n"
           << "Split: " << movie.split << "\n"
           << "Synopsis Source: " << movie.synopsis_source << "\n";
        return os;
    }
};

#endif