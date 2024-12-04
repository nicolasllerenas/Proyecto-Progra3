#include "treeHandle.h"
#include "csvHandle.h"

int main(){
    const std::string inputFile = "mpst_full_data.csv";
    const std::string outputFile = "mpst_full_data_modified.csv";
    processCSV(inputFile, outputFile);
    std::vector<MovieData> movies = readCSV(outputFile);
    // printMovies(movies); Imprime todo. Demora mil años no lo ejecutes xd, úsalo para mostrar resultados con un vector más pequeño

    // Crear el árbol y poner las películas ahí
    MovieSearchTree tree;
    for (const auto& movie : movies) {
        tree.insert(movie);
    }

    //Demostración de uso 
    //Borrar una vez esté la interfaz
    //Hay que implementar dos modos de búsqueda, que el usuario elija si por string o por tag

    // Buscar por string (ej. "baby")
    std::string searchTerm = "baby";
    std::vector<MovieData> searchResults = tree.search(searchTerm);

    // Imprimir el top 5
    std::cout << "Top 5 Search Results for: \"" << searchTerm << std::endl;
    printMovies(searchResults);

    // buscar por tag (ej. "violence")
    std::string searchTermTags = "violence";
    std::vector<MovieData> tagSearchResults = tree.searchByTags(searchTermTags);

    // Imprimir el top 5 por tag
    std::cout << "Top 5 Search Results for Tags: \"" << searchTermTags<<std::endl;
    printMovies(tagSearchResults);
    return 0;
}
