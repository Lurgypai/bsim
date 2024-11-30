#include "readers.h"
#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "tle_mover.h"

static std::vector<std::string> split_string(const std::string line, const std::string delimiter) {
    std::vector<std::string> result;
    std::string remainder = line;
    size_t idx = remainder.find(delimiter);
    while (idx != std::string::npos) {
        result.push_back(remainder.substr(0, idx));
        remainder = remainder.substr(idx + delimiter.size(), remainder.size());
        idx = remainder.find(delimiter);
    }
    result.push_back(remainder);
    return result;
}

std::vector<Station> readGroundStations(const std::string& fileName) {
    std::vector<Station> stations;

    std::ifstream file{fileName};
    if(!file.good()) {
        std::cout << "Error: Unable to open file \"" << fileName << "\"\n";
        throw std::exception{};
    }

    std::string line;
    while(std::getline(file, line)) {
        auto vals = split_string(line, ",");
        stations.push_back(Station{});
        stations.back().pos = {
            std::stof(vals[5]),
            std::stof(vals[6]),
            std::stof(vals[7])
        };
    }

    return stations;
}

std::vector<Station> readSatellites(const std::string& fileName) {
    std::vector<Station> stations;

    std::ifstream file{fileName};
    if(!file.good()) {
        std::cout << "Error: Unable to open file \"" << fileName << "\"\n";
        throw std::exception{};
    }

    std::string line;
    std::getline(file, line); //skip the first line
    std::string tle1;
    int linedex = 0;
    for(std::string line; std::getline(file, line); ++linedex) {
        linedex %= 3;

        if(linedex == 0) {
            continue;
        }

        // store first tle
        if(linedex == 1) {
            tle1 = line;
            continue;
        }

        //store second tle and create satellite
        stations.push_back(Station{});
        stations.back().loadMover<TleMover>(tle1, line);
    }

    return stations;
}
