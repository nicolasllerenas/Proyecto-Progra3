// trie.cpp
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
        current->movies.push_back(movie);
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
            return {};
        }
        current = current->children[c];
    }
    
    return current->movies;
}
