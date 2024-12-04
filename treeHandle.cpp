#include "treeHandle.h"

// insertar un nodo
void MovieSearchTree::insert(const MovieData& movie) {
        root = insertRec(root, movie);
}

std::vector<MovieData> MovieSearchTree::search(const std::string& term) {
    std::vector<MovieData> result;
    searchRec(root, term, result);

    std::sort(result.begin(), result.end(), [this, &term](const MovieData& a, const MovieData& b) {
        return countOccurrences(a, term) > countOccurrences(b, term);
    });

    // Retornar top 5
    if (result.size() > 5) {
        result.resize(5);
    }

    return result;
}

// Insertar una película en el árbol
TreeNode* MovieSearchTree::insertRec(TreeNode* node, const MovieData& movie) {
    if (node == nullptr) {
        return new TreeNode(movie);
    }

    // Comparar títulos (no importa mayúsculas)
    if (compareTitles(movie.title, node->data.title) < 0) {
        node->left = insertRec(node->left, movie);
    } else {
        node->right = insertRec(node->right, movie);
    }

    return node;
}

// Buscar plículas en título o sinopsis (sin imporar mayúsculas)
void MovieSearchTree::searchRec(TreeNode* node, const std::string& term, std::vector<MovieData>& result) {
    if (node == nullptr) {
        return;
    }

    // Añadirla si se encuentra
    if (containsTerm(node->data.title, term) || containsTerm(node->data.plot_synopsis, term)) {
        result.push_back(node->data);
    }

    // Buscar en la izquierda o la derecha
    if (compareTitles(term, node->data.title) < 0) {
        searchRec(node->left, term, result);
    } else {
        searchRec(node->right, term, result);
    }
}

// Comparar títulos sin importar mayúsculas
int MovieSearchTree::compareTitles(const std::string& a, const std::string& b) {
    std::string aLower = a;
    std::string bLower = b;
    std::transform(aLower.begin(), aLower.end(), aLower.begin(), ::tolower);
    std::transform(bLower.begin(), bLower.end(), bLower.begin(), ::tolower);
    return aLower.compare(bLower);
}

// Ver si una cadena contiene lo buscado sin importar mayúsculas
bool MovieSearchTree::containsTerm(const std::string& str, const std::string& term) {
    std::string strLower = str;
    std::string termLower = term;
    std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
    std::transform(termLower.begin(), termLower.end(), termLower.begin(), ::tolower);
    return strLower.find(termLower) != std::string::npos;
}

// Contar ocurrencias en el título o sinopsis
// Para rankear las películas solo se mira cuántas veces aparece el término xd
// tanto en title como sinopsis. Más adelante a ver si se puede poner algo mpas sofisticado
// pero este funciona
int MovieSearchTree::countOccurrences(const MovieData& movie, const std::string& term) {
    int count = 0;
    count += countInString(movie.title, term);
    count += countInString(movie.plot_synopsis, term);
    return count;
}

// Contar ocurrencias de un término dentro de una string
int MovieSearchTree::countInString(const std::string& str, const std::string& term) {
    int count = 0;
    size_t pos = 0;
    while ((pos = str.find(term, pos)) != std::string::npos) {
        ++count;
        pos += term.length(); // Pasar al siguiente match
    }
    return count;
}

// Buscar tags sin importar mayúsculas
std::vector<MovieData> MovieSearchTree::searchByTags(const std::string& term) {
    std::vector<MovieData> result;
    searchByTagsRec(root, term, result);

    // Ordenar los resultados por cantidad de ocurrencias
    std::sort(result.begin(), result.end(), [this, &term](const MovieData& a, const MovieData& b) {
        return countOccurrencesInTags(a, term) > countOccurrencesInTags(b, term);
    });

    // Retornar top 5
    if (result.size() > 5) {
        result.resize(5);
    }

    return result;
}

// Búsqueda recursiva de películas por tags (usado por la otra función)
void MovieSearchTree::searchByTagsRec(TreeNode* node, const std::string& term, std::vector<MovieData>& result) {
    if (node == nullptr) {
        return;
    }

    // Añadir película si se encuentra el tag
    if (containsTerm(node->data.tags, term)) {
        result.push_back(node->data);
    }

    // Buscar izquierda o derecha
    if (compareTitles(term, node->data.title) < 0) {
        searchByTagsRec(node->left, term, result);
    } else {
        searchByTagsRec(node->right, term, result);
    }
}

// Ccontar ocurrencias en los tags
int MovieSearchTree::countOccurrencesInTags(const MovieData& movie, const std::string& term) {
    return countInString(movie.tags, term);
}