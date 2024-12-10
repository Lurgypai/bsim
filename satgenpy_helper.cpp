#include "satgenpy_helper.h"

#include <fstream>

std::vector<std::string> RouteHelper::split_string(const std::string& line, const std::string& delimiter) {
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

LinkMap RouteHelper::readRoutesFile(const std::string& fileName) {
    // std::cout << "Reading routes file " << fileName << '\n';
    std::ifstream stateFile{fileName};
    if(!stateFile.good()) {
        std::string e = "Error: Unable to open file \"" + fileName + "\"\n";
        throw std::runtime_error{e};
    }

    LinkMap map;

    std::string line;
    while(std::getline(stateFile, line)) {
        auto v = split_string(line, ",");
        NetDeviceId sourceId = std::stoi(v[0]);
        NetDeviceId targetId = std::stoi(v[1]);
        NetDeviceId nextId = std::stoi(v[2]);
        Link l{sourceId, nextId};
        map[sourceId].push_back(l);
    }

    return map;
}
