#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera.h"
#include "types.h"
#include "station.h"

class LinkManager3d {
public:
    void loadLinks(const std::string& fileName);
    void draw(const std::vector<Station> stations, sf::RenderWindow& window, const Camera& cam);
    float scale;
private:
    std::unordered_map<NetDeviceId, std::vector<NetDeviceId>> links;
};
