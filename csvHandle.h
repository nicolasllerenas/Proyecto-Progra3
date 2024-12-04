#ifndef CSVHANDLE_H
#define CSVHANDLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

#include "MovieData.h"

void processCSV(const std::string&, const std::string&);
std::vector<MovieData> readCSV(const std::string&);
void printMovies(const std::vector<MovieData>&);

#endif