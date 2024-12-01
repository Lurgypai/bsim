#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <chrono>

#include <cmath>
#include <iostream>

#include "station.h"
#include "satellite_renderer.h"
#include "readers.h"
#include "link_manager_3d.h"

constexpr float twopi = 2.f * 3.1415926535898f;

constexpr Milliseconds simTime  = 200 * 1000;
// constexpr Milliseconds simTime  = 200 * 10000000;

using namespace std::chrono;

int main(int argc, char** argv) {

    std::vector<Station> groundStations = readGroundStations("ground_stations.txt");
    std::vector<Station> satellites = readSatellites("tles.txt");
    std::vector<Station> stations;
    stations.reserve(groundStations.size() + satellites.size());
    stations.insert(stations.end(), satellites.begin(), satellites.end());
    stations.insert(stations.end(), groundStations.begin(), groundStations.end());
    for(int i = 0; i != stations.size(); ++i) {
        stations[i].deviceId = i;
    }

    std::cout << "There are " << satellites.size() << " satellites and " << groundStations.size() << " ground stations making " << stations.size() << " total stations.\n";

    LinkManager3d links;
    links.loadLinks("isls.txt");
    links.setRouteFolder("routes", 50, simTime);

    SatelliteRenderer renderer;
    renderer.scale = 1.f / 20000;
    links.scale = renderer.scale;
    renderer.genSatellites(satellites.size(), 4.f, sf::Color::Red);
    renderer.genSatellites(groundStations.size(), 4.f, sf::Color::White, satellites.size());
    renderer.cam.pos = {0, 0, -2000};
    renderer.cam.setOrientation({0, 0, 0}, {0, 0, 1});

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Satellites", sf::Style::None);
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1920, 1080));
    window.setView(view);

    float horizAngle = 0.f;;
    float height = 0.f;
    float rotSpeed = 0.05;
    float maxHeight = 20;

    //decouple tick rate and update rate

    Milliseconds elapsed = 0;
    // amount of time to tick the simulation forward
    Milliseconds tickAmount = 50 * 50;
    // amount of time between updates to the simulation
    auto physics_rate = milliseconds(50); // when this and the above match you're running in real time
    // frame rate
    auto render_rate = milliseconds(16);

    auto last_physics = high_resolution_clock::now();
    auto last_render = last_physics;


    for(;;) {
        auto now = high_resolution_clock::now();

        if(duration_cast<milliseconds>(now - last_physics) > physics_rate) {
            last_physics = now;

            elapsed += tickAmount;
            if(elapsed < simTime) {
            // std::cout << "elapsed " << elapsed << '\n';
                for(auto& station : stations) {
                    station.update(tickAmount);
                }

                links.updateRoutes(tickAmount);
            }
        }

        if(duration_cast<milliseconds>(now - last_render) > render_rate) {
            last_render = now;
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }


            int horizDir = 0;
            int vertDir = 0;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ++horizDir;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) --horizDir;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) ++vertDir;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) --vertDir;


            horizAngle += horizDir * rotSpeed;
            if(horizAngle > twopi) horizAngle -= twopi;

            height += vertDir * 0.05;
            if(height > maxHeight) height = maxHeight;
            else if (height < -maxHeight) height = -maxHeight;

            renderer.cam.pos = {std::cos(horizAngle), std::sin(horizAngle), height};
            renderer.cam.setOrientation({0, 0, 0}, {0, 0, 1});

            window.clear(sf::Color::Black);
            renderer.drawSatellites(stations, window);
            // links.drawLinks(stations, window, renderer.cam);
            links.drawRoutes(stations, window, renderer.cam);
            window.display();
        }
    }

    return 0;
}
