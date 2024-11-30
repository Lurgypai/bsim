#pragma once

#include <SFML/Graphics/Color.hpp>
#include <list>

#include <SFML/Graphics.hpp>

#include "station.h"
#include "camera.h"

class SatelliteRenderer {
public:
    SatelliteRenderer();
    void genSatellites(int count, float radius, const sf::Color& color, int idOffset = 0);
    void drawSatellites(std::vector<Station>& stations, sf::RenderWindow& window);
    Camera cam;
    float scale;
private:
    struct AssignedCircle {
        NetDeviceId id;
        Vec3::Type z;
        sf::CircleShape shape;


    };
    std::list<AssignedCircle> satellites;
};
