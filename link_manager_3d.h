#pragma once

#include <SFML/Graphics/Rect.hpp>
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
    void setRouteFolder(const std::string& folderName, Milliseconds rate_, Milliseconds duration_);
    void updateRoutes(Milliseconds delta);
    void drawLinks(const std::vector<Station>& stations, sf::RenderWindow& window, const Camera& cam);
    void drawRoutes(const std::vector<Station>& stations, sf::RenderWindow& window, const Camera& cam);
    float scale;
private:
    Milliseconds rate;
    Milliseconds remainder;
    Milliseconds elapsed;
    Milliseconds duration;

    std::string routeFolder;
    std::unordered_map<NetDeviceId, std::vector<NetDeviceId>> links;
    std::unordered_map<NetDeviceId, std::unordered_map<NetDeviceId, NetDeviceId>> routes;
};
