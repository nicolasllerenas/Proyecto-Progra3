// trie.h
#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "movie.h"

class TrieNode {
public:
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    std::vector<std::shared_ptr<Movie>> movies;

    TrieNode();
};

class Trie {
private:
    std::shared_ptr<TrieNode> root;

public:
    Trie();
    void insert(const std::string& word, std::shared_ptr<Movie> movie);
    std::vector<std::shared_ptr<Movie>> search(const std::string& word);
    std::vector<std::shared_ptr<Movie>> searchByPrefix(const std::string& prefix);
};
