#include "MovieData.h"
#include <vector>
#include <algorithm>

// struct de nodo
struct TreeNode {
    MovieData data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(MovieData movieData) : data(movieData), left(nullptr), right(nullptr) {}
};

// BÁrbol de búsqueda binaria de películas
class MovieSearchTree {
private:
    TreeNode* root;
public:
    MovieSearchTree() : root(nullptr) {}

    void insert(const MovieData&);
    std::vector<MovieData> search(const std::string&);
    TreeNode* insertRec(TreeNode*, const MovieData&);
    void searchRec(TreeNode* , const std::string& , std::vector<MovieData>&);
    int compareTitles(const std::string&, const std::string&);
    bool containsTerm(const std::string&, const std::string&);
    int countOccurrences(const MovieData&, const std::string&);
    int countInString(const std::string&, const std::string&);
    std::vector<MovieData> searchByTags(const std::string&);
    void searchByTagsRec(TreeNode*, const std::string&, std::vector<MovieData>&);
    int countOccurrencesInTags(const MovieData&, const std::string&);
};