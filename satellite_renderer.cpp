#include "satellite_renderer.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

SatelliteRenderer::SatelliteRenderer() :
    cam{},
    scale{},
    satellites{}
{
    satellites.emplace_back(
        AssignedCircle{-1, 0., sf::CircleShape{300}}
    );
    auto& earth = satellites.back();
    earth.shape.setFillColor(sf::Color::Blue);
    earth.shape.setOrigin(earth.shape.getRadius(), earth.shape.getRadius());
}

void SatelliteRenderer::genSatellites(int count, float radius, const sf::Color& color, int idOffset) {
    for(int id = 0; id != count; ++id) {
        satellites.emplace_back(
                AssignedCircle{id + idOffset, 0., sf::CircleShape{radius}}
        );

        auto& sat = satellites.back();
        sat.shape.setFillColor(color);
        sat.shape.setOrigin(sat.shape.getRadius(), sat.shape.getRadius());
    }
}

void SatelliteRenderer::drawSatellites(std::vector<Station>& stations, sf::RenderWindow& window) {
    for(auto& circle : satellites) {
        if(circle.id < 0) continue; // earth
        auto& station = stations[circle.id];

        Vec3 pos = cam.project(station.pos * scale);
        circle.z = pos.z;
        circle.shape.setPosition(pos.x, pos.y);
    }
    satellites.sort(
        [](const AssignedCircle& a, const AssignedCircle& b) -> bool {
        return a.z < b.z;
    });

    for(auto& satellite : satellites) {
        window.draw(satellite.shape);
    }
}
