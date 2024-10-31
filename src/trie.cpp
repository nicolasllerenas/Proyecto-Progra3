#include "trie.h"
#include <algorithm>

TrieNode::TrieNode() : isEndOfWord(false) {}

Trie::Trie() : root(std::make_shared<TrieNode>()) {}

void Trie::insert(const std::string& word, std::shared_ptr<Movie> movie) {
    auto current = root;
    std::string lowercaseWord = word;
    std::transform(lowercaseWord.begin(), lowercaseWord.end(), 
                  lowercaseWord.begin(), ::tolower);
    
    for (char c : lowercaseWord) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = std::make_shared<TrieNode>();
        }
        current = current->children[c];
        
        // Verificar si ya existe la película en este nodo antes de añadirla
        if (std::find(current->movies.begin(), current->movies.end(), movie) == current->movies.end()) {
            current->movies.push_back(movie);
        }
    }
    current->isEndOfWord = true;
}

std::vector<std::shared_ptr<Movie>> Trie::search(const std::string& word) {
    auto current = root;
    std::string lowercaseWord = word;
    std::transform(lowercaseWord.begin(), lowercaseWord.end(), 
                  lowercaseWord.begin(), ::tolower);
    
    for (char c : lowercaseWord) {
        if (current->children.find(c) == current->children.end()) {
            return {}; // Retorna vacío si el término de búsqueda no se encuentra
        }
        current = current->children[c];
    }

    // Calcular y ordenar las películas según el importance_score
    std::vector<std::shared_ptr<Movie>> result = current->movies;
    for (auto& movie : result) {
        movie->calculateImportance(word); // Calcula la importancia basada en el término de búsqueda
    }
    
    // Ordenar el resultado por importance_score en orden descendente
    std::sort(result.begin(), result.end(), [](const std::shared_ptr<Movie>& a, const std::shared_ptr<Movie>& b) {
        return a->getImportanceScore() > b->getImportanceScore();
    });

    return result;
}