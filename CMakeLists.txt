cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(ProyectoProgra3)

# Estándar de C++
set(CMAKE_CXX_STANDARD 20)

# Incluir directorios de cabecera
include_directories(include)

# Buscar todos los archivos fuente en src/
file(GLOB SOURCES "src/*.cpp")

# Compilar el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Definir el directorio de salida de binarios en la carpeta build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)
