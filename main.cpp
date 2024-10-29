#include <iostream>   // Librería para entrada y salida estándar
#include <fstream>    // Librería para trabajar con archivos
#include <sstream>    // Librería para manipular cadenas de texto (para procesar cada línea del CSV)
#include <vector>     // Librería para el uso de vectores
#include <string>     // Librería para trabajar con cadenas de texto
#include <algorithm>  // Librería para usar funciones como `find` en strings

using namespace std;

// Clase Pelicula que representa la estructura de cada película en el sistema
class Pelicula {
public:
    string titulo;
    string sinopsis;
    vector<string> tags;

    // Constructor que inicializa los atributos de la película
    Pelicula(const string& t, const string& s, const vector<string>& tg) 
        : titulo(t), sinopsis(s), tags(tg) {}

    // Función para verificar si una palabra está en el título o sinopsis
    bool contienePalabra(const string& palabra) const {
        return (titulo.find(palabra) != string::npos || sinopsis.find(palabra) != string::npos);
    }
};

// Clase Nodo que representa un nodo en el árbol binario de búsqueda
class Nodo {
public:
    Pelicula pelicula;
    Nodo* izquierda;
    Nodo* derecha;

    // Constructor para inicializar el nodo con una película
    Nodo(const Pelicula& p) : pelicula(p), izquierda(nullptr), derecha(nullptr) {}
};

// Clase ArbolPeliculas que representa el árbol binario de búsqueda para almacenar y buscar películas
class ArbolPeliculas {
private:
    Nodo* raiz; // Puntero a la raíz del árbol

    // Función recursiva para insertar una película en el árbol
    Nodo* insertar(Nodo* nodo, const Pelicula& pelicula) {
        if (!nodo) return new Nodo(pelicula); // Si el nodo es nulo, crea uno nuevo
        // Ordena las películas por título para insertarlas en el árbol
        if (pelicula.titulo < nodo->pelicula.titulo)
            nodo->izquierda = insertar(nodo->izquierda, pelicula);
        else
            nodo->derecha = insertar(nodo->derecha, pelicula);
        return nodo;
    }

    // Función recursiva para buscar películas que contienen una palabra en título o sinopsis
    void buscarPorPalabra(Nodo* nodo, const string& palabra, vector<Pelicula>& resultados) const {
        if (!nodo) return; // Caso base: nodo vacío
        // Si el título o sinopsis contiene la palabra, añade la película a los resultados
        if (nodo->pelicula.contienePalabra(palabra))
            resultados.push_back(nodo->pelicula);
        // Búsqueda en los subárboles izquierdo y derecho
        buscarPorPalabra(nodo->izquierda, palabra, resultados);
        buscarPorPalabra(nodo->derecha, palabra, resultados);
    }

public:
    ArbolPeliculas() : raiz(nullptr) {} // Constructor que inicializa la raíz como nula

    // Método público para insertar una película en el árbol
    void insertar(const Pelicula& pelicula) {
        raiz = insertar(raiz, pelicula);
    }

    // Método público para buscar películas por palabra
    vector<Pelicula> buscarPorPalabra(const string& palabra) const {
        vector<Pelicula> resultados;
        buscarPorPalabra(raiz, palabra, resultados);
        return resultados;
    }
};

// Función para cargar las películas desde un archivo CSV al árbol
void cargarPeliculasDesdeCSV(const string& filename, ArbolPeliculas& arbol) {
    ifstream archivo(filename); // Abre el archivo CSV
    string linea;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) { // Lee cada línea del archivo
            stringstream ss(linea); // Procesa la línea con stringstream
            string titulo, sinopsis, tag;
            vector<string> tags;

            getline(ss, titulo, ','); // Obtiene el título
            getline(ss, sinopsis, ','); // Obtiene la sinopsis

            // Lee los tags restantes (si existen) y los agrega al vector
            while (getline(ss, tag, ',')) {
                tags.push_back(tag);
            }

            // Crea un objeto Pelicula y lo inserta en el árbol
            Pelicula pelicula(titulo, sinopsis, tags);
            arbol.insertar(pelicula);
        }
        archivo.close(); // Cierra el archivo después de leer
    } else {
        cerr << "No se pudo abrir el archivo CSV." << endl;
    }
}

// Función para mostrar la lista de películas encontradas en la búsqueda
void mostrarPeliculas(const vector<Pelicula>& peliculas) {
    for (const auto& pelicula : peliculas) {
        cout << "Titulo: " << pelicula.titulo << endl;
        cout << "Sinopsis: " << pelicula.sinopsis << endl;
        cout << "Tags: ";
        for (const auto& tag : pelicula.tags) {
            cout << tag << " ";
        }
        cout << "\n-----------------------\n";
    }
}

int main() {
    ArbolPeliculas arbol;
    cargarPeliculasDesdeCSV("mpst_full_data.csv", arbol); // Carga el archivo CSV en el árbol

    cout << "Ingrese una palabra para buscar en las peliculas: ";
    string palabra;
    getline(cin, palabra);

    // Busca las películas que contienen la palabra ingresada en el título o sinopsis
    vector<Pelicula> resultados = arbol.buscarPorPalabra(palabra);

    // Muestra los resultados de la búsqueda
    if (!resultados.empty()) {
        cout << "Peliculas encontradas:\n";
        mostrarPeliculas(resultados);
    } else {
        cout << "No se encontraron peliculas que contengan la palabra '" << palabra << "'." << endl;
    }

    return 0;
}
