#include "link_manager_3d.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>

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

void LinkManager3d::draw(const std::vector<Station> stations, sf::RenderWindow& window, const Camera& cam) {
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
