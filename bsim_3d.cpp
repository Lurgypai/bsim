#include <SFML/Graphics.hpp>

#include <chrono>
#include <cmath>

#include "topology.h"
#include "base_netdevice_loader.h"
#include "satgenpy_station_loader.h"
#include "satgenpy_link_loader.h"
#include "satgenpy_router_impl.h"

constexpr float twopi = 2.f * 3.1415926535898f;

constexpr Milliseconds simTime  = 200 * 1000;
// constexpr Milliseconds simTime  = 200 * 10000000;

using namespace std::chrono;

int main(int argc, char** argv) {


    Topology topology;
    auto& stations = topology.setStationLoader<SatgenpyStationLoader>();
    topology.setNetDeviceLoader<BaseNetDeviceLoader>(stations.getSatelliteCount() + stations.getGroundStationCount());
    topology.setLinkLoader<SatgenpyLinkLoader>(50);
    topology.setRouterImpl<SatgenpyRouterImpl>(50);
    topology.loadAll();

    topology.prepareRendering();
    topology.cam.scale = 1.f / 20000;
    topology.cam.pos = {0, 0, -2000};
    topology.cam.setOrientation({0, 0, 0}, {0, 0, 1});  

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
                topology.update(tickAmount);
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

            topology.cam.pos = {std::cos(horizAngle), std::sin(horizAngle), height};
            topology.cam.setOrientation({0, 0, 0}, {0, 0, 1});

            window.clear(sf::Color::Black);

            topology.draw(window);
            window.display();
        }
    }

    return 0;
}
