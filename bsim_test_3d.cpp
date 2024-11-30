#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include <cmath>

#include "station.h"
#include "satellite_renderer.h"

constexpr float twopi = 2.f * 3.1415926535898f;

class StationMoverTest : public StationMover {
public:
    float speed;
    float offset;
    float radius;

    virtual ~StationMoverTest() {};
    virtual void moveStation(Station& station, Milliseconds delta) override {
        time += delta;
        station.pos.x = std::cos(time * speed + offset) * radius;
        station.pos.z = std::sin(time * speed + offset) * radius;
    };

    virtual std::unique_ptr<StationMover> clone() const override {
        return std::make_unique<StationMoverTest>(*this);
    }
private:
    Milliseconds time;
};

int main(int argc, char** argv) {

    std::vector<Station> stations{};
    stations.resize(15);
    for(int i; i != stations.size(); ++i) {
        auto& station = stations[i];

        station.deviceId = i;

        auto& mover = station.loadMover<StationMoverTest>();
        mover.speed = 0.001f;
        mover.radius = 100.f;
        mover.offset = (twopi / 5) * static_cast<int>(station.deviceId / 3);
        if(i % 3 == 0) station.pos.y = -40;
        if(i % 3 == 2) station.pos.y = 40;
    }

    SatelliteRenderer renderer;
    renderer.scale = 1.f;
    renderer.genSatellites(stations.size(), 20.f, sf::Color::White);
    renderer.cam.pos = {0, 0, -100};
    renderer.cam.setOrientation({0, 0, 0}, {0, -1, 0});

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Satellites", sf::Style::None);
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1920, 1080));
    window.setView(view);

    float horizAngle = 0.f;;
    float height = 0.f;
    float rotSpeed = 0.001;

    int tick = 0;
    for(;;) {
        /*
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        */

        for(auto& station : stations) {
            station.update(1);
        }

        int horizDir = 0;
        int vertDir = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ++horizDir;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) --horizDir;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) ++vertDir;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) --vertDir;


        horizAngle += horizDir * rotSpeed;
        if(horizAngle > twopi) horizAngle -= twopi;

        height += vertDir * 0.01;
        if(height > 50) height = 50;
        else if (height < -50) height = -50;

        renderer.cam.pos = {std::cos(horizAngle), height, std::sin(horizAngle)};
        renderer.cam.setOrientation({0, 0, 0}, {0, -1, 0});

        window.clear(sf::Color::Black);
        renderer.drawSatellites(stations, window);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
