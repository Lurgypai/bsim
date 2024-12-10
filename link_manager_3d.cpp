#include "link_manager_3d.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

void LinkManager3d::loadLinks(const std::string& fileName) {

    std::ifstream file{fileName};
    if(!file.good()) {
        std::cout << "Error: Unable to open file \"" << fileName << "\"\n";
        throw std::exception{};
    }

    std::string line;
    while(std::getline(file, line)) {
        auto pair = split_string(line, " ");
        NetDeviceId source = std::stoi(pair[0]);
        NetDeviceId dest = std::stoi(pair[1]);
        links[source].push_back(dest);
    }
}

static void readRoutesFile(const std::string& fileName, std::unordered_map<NetDeviceId, std::unordered_map<NetDeviceId, NetDeviceId>>& routes) {
    // std::cout << "Reading routes file " << fileName << '\n';
    std::ifstream stateFile{fileName};
    if(!stateFile.good()) {
        std::string e = "Error: Unable to open file \"" + fileName + "\"\n";
        throw std::runtime_error{e};
    }

    std::string line;
    while(std::getline(stateFile, line)) {
        auto v = split_string(line, ",");
        NetDeviceId sourceId = std::stoi(v[0]);
        NetDeviceId targetId = std::stoi(v[1]);
        NetDeviceId nextId = std::stoi(v[2]);
        routes[sourceId][targetId] = nextId;
    }
}

void LinkManager3d::setRouteFolder(const std::string& folderName, Milliseconds rate_, Milliseconds duration_) {
    routeFolder = folderName;
    rate = rate_;
    duration = duration_;

    remainder = 0;
    elapsed = 0;

    readRoutesFile(folderName + "/fstate_0.txt", routes);
}

void LinkManager3d::updateRoutes(Milliseconds delta) {
    remainder += delta;
    while(remainder > rate) {
        remainder -= rate;
        elapsed += rate;

        std::string targetFile{routeFolder + "/fstate_" + std::to_string(elapsed) + "000000.txt"};
        readRoutesFile(targetFile, routes);
    }
};

void LinkManager3d::drawLinks(const std::vector<Station>& stations, sf::RenderWindow& window, const Camera& cam) {
    for(auto& pair : links) {
        const Station& source = stations[pair.first];
        Vec3 p1 = cam.project(source.pos);
        if(p1.z < 0) continue;

        p1 *= scale;
        for(auto& destId : pair.second) {
            const Station& dest = stations[destId];
            Vec3 p2 = cam.project(dest.pos);
            if(p2.z < 0) continue;
            p2 *= scale;

            sf::Vertex line[]{
                sf::Vertex{sf::Vector2f{static_cast<float>(p1.x), static_cast<float>(p1.y)}, sf::Color::White},
                sf::Vertex{sf::Vector2f{static_cast<float>(p2.x), static_cast<float>(p2.y)}, sf::Color::White}
            };

            window.draw(line, 2, sf::Lines);
        }
    }
}

void LinkManager3d::drawRoutes(const std::vector<Station>& stations, sf::RenderWindow& window, const Camera& cam) {
    for(const auto& [sourceId, route] : routes) {
        const Station& source = stations[sourceId];
        Vec3 p1 = cam.project(source.pos);
        if(p1.z < 0) continue;
        p1 *= scale;

        for(const auto& [destId, nextId] : route) {
            const Station& dest = stations[nextId];
            Vec3 p2 = cam.project(dest.pos);
            if(p2.z < 0) continue;
            p2 *= scale;

            auto color = sf::Color::Red;
            constexpr int groundStationThreshold = 1584;
            if(sourceId >= groundStationThreshold || nextId >= groundStationThreshold) color = sf::Color::Green;

            sf::Vertex line[]{
                sf::Vertex{sf::Vector2f{static_cast<float>(p1.x), static_cast<float>(p1.y)}, color},
                sf::Vertex{sf::Vector2f{static_cast<float>(p2.x), static_cast<float>(p2.y)}, color}
            };

            window.draw(line, 2, sf::Lines);
        }
    }
}
