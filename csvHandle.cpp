#include "csvHandle.h"

//El tema con el csv original es que está recontra feo xd, esta función lo arregla
//Lo que hace es asegurar que todas las líneas empiecen por el código tt102312
void processCSV(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    std::ofstream outFile(outputFile);

    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return;
    }
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }

    std::string line;
    std::string buffer;
    std::regex codePattern("^tt\\d{7}");

    while (std::getline(inFile, line)) {
        if (std::regex_search(line, codePattern)) {
            // Si una línea empieza con el código, poner todo en búfer para después
            if (!buffer.empty()) {
                outFile << buffer << "\n";
            }
            buffer = line; // nuevo búfer
        } else {
            buffer += line; // append al buffer actual
        }
    }

    // Poner todo el contenido que falte en el búfer
    if (!buffer.empty()) {
        outFile << buffer << "\n";
    }

    inFile.close();
    outFile.close();
    //std::cout << "Processing complete. Output written to " << outputFile << std::endl;
}

//leer el csv
std::vector<MovieData> readCSV(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<MovieData> rows;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return rows;
    }

    std::string line;

    // Saltarse el header imdb_id, title, plot_synopsis, etc
    if (std::getline(file, line)) {
        //std::cout << "Header: " << line << std::endl; // Opcional: imprimir el header. Está para debugging nomás
    }
    
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        MovieData row;
        std::string cell;

        // Función para manejar las citas dentro de las comas
        // la vaina es que a la sinopsis está escrita "así de esta manera"
        // para que no se lean las comas dentro. Sin embargo hay un problema porque
        // a veces en el texto citan diálogo ""así"". Esto lo maneja
        // también funciona con los tags que están "así, pe, mano, con, comas"
        auto readQuotedField = [](std::istringstream& stream) {
            std::string field;
            if (stream.peek() == '\"') {
                stream.get(); // borrar primera "
                std::string temp;
                while (std::getline(stream, temp, '\"')) {
                    field += temp;
                    if (stream.peek() == '\"') {
                        stream.get(); // borrar el " doble
                        field += '\"'; // volver a añadir para mantener la cita
                    } else {
                        break; // terminó la cita
                    }
                }
                if (stream.peek() == ',') stream.get();
                // borrar las comas si hay. No creo que pase pero no voy a revisar 40000 películas para comprobarlo. Lo dejo por si acaso
            } else {
                std::getline(stream, field, ','); // leer hasta la última coma
            }
            return field;
        };

        // Leer los campos
        std::getline(stream, row.imdb_id, ',');
        std::getline(stream, row.title, ',');
        row.plot_synopsis = readQuotedField(stream); // Lidiar caso especial
        row.tags = readQuotedField(stream);          // Lidiar caso especial
        std::getline(stream, row.split, ',');
        std::getline(stream, row.synopsis_source, ',');

        rows.push_back(row);
    }

    file.close();
    return rows;
}

// función para imprimir
void printMovies(const std::vector<MovieData>& movies) {
    int index = 1;
    for (const auto& movie : movies) {
        std::cout << index<<".\n";
        index++;
        std::cout << movie;
        for (int i = 0; i < 30; i++)
        {
            std::cout<<"-";
        }
        std::cout<<std::endl;
    }
}