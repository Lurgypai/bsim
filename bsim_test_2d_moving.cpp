#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdlib>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include "netdevice_manager.h"
#include "router.h"
#include "station.h"
#include "station_mover_satellite2d.h"
#include "link_manager2d.h"

class TestRouter : public RouterImpl {
public:
    TestRouter(NetDeviceManager& deviceManager_, LinkManager2d& linkManager_, std::vector<Station>& stations_) :
        deviceManager{deviceManager_},
        linkManager{linkManager_},
        stations{stations_}
    {}
private:
    NetDeviceManager& deviceManager;
    LinkManager2d& linkManager;
    std::vector<Station>& stations;

    NetDevice* findNext(NetDeviceId curId, NetDeviceId destId) override {
        Station& destStation = stations[destId];
        
        NetDevice* nextDevice = nullptr;
        float minDist = 10000.f;

        auto links = linkManager.getLinks().find(curId);

        if(links == linkManager.getLinks().end()) return nullptr;

        for(auto& nextStationId : linkManager.getLinks().at(curId)) {
            // std::cout << "Exploring link " << curId << " → " << nextStationId;
            auto& nextStation = stations[nextStationId];
            
            float curDist = nextStation.pos.dist(destStation.pos);
            if(curDist >= minDist) continue;
            minDist = curDist;
            nextDevice = &deviceManager.getNetDevice(nextStationId);
        }
        return nextDevice;
    };
};

int main(int argc, char** argv) {

    NetDeviceManager manager;
    manager.genNetDevices(37);

    LinkManager2d linkManager;
    std::vector<Station> stations;

    Router routerBase;
    routerBase.loadImpl<TestRouter>(manager, linkManager, stations);
    NetDevice::router = &routerBase;

    stations.resize(37);
    for(int i = 0; i != 35; ++i) {
        auto& station = stations[i];
        station.deviceId = i;

        auto& mover = station.loadMover<StationMoverSatellite2d>();

        constexpr float twopi = 2.f * 3.1415926535898f;

        // float baseSpeed = .00000416666f;
        float baseSpeed = .0001f;

        if(i < 5) {
            mover.speed = baseSpeed;
            mover.radius = 100.f;
            mover.offset = (twopi / 5) * i;
        }
        else if (i < 15) {
            mover.speed = baseSpeed * 0.9;
            mover.radius = 200.f;
            mover.offset = (twopi / 10) * (i - 5) + (twopi / 20);
        }
        else {
            mover.speed = baseSpeed * 0.8;
            mover.radius = 300.f;
            mover.offset = (twopi / 20) * (i - 15) + (twopi / 40);
        }
    }

    stations[35].deviceId = 35;
    stations[35].pos = {-280, -280, 0};

    stations[36].deviceId = 36;
    stations[36].pos = {280, 280, 0};

    constexpr int PACKET_COUNT = 100;
    for(int i = 0; i != PACKET_COUNT; ++i) {
        manager.getNetDevice(35).queue(Packet{i, 36});
    }

    /* ===== RENDERING ===== */
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Satellites", sf::Style::None);
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1920, 1080));
    window.setView(view);

    while(manager.getNetDevice(36).getReceivedCount() != PACKET_COUNT) {
        for(auto& station : stations) {
            station.update(1);
        }
        linkManager.update(stations);
        manager.updateDevices(1);

        /* RENDERING */
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        /* END RENDERING */
        
        /* RENDER LINES */
        for(auto& pair : linkManager.getLinks()) {
            auto& sourceStation = stations[pair.first];
            for(auto& destId : pair.second) {
                auto& destStation = stations[destId];

                auto color = sf::Color::White;
                auto& sourceDev = manager.getNetDevice(sourceStation.deviceId);
                auto& destDev = manager.getNetDevice(destId);

                if((sourceDev.isSending() && destDev.isReceiving() && sourceDev.getNextDevice()->getId() == destId)
                    || destDev.isSending() && sourceDev.isReceiving() && destDev.getNextDevice()->getId() == pair.first) {
                    color = sf::Color::Green;
                }

                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(sourceStation.pos.x, sourceStation.pos.y), color),   // Start point (100, 100) with red color
                    sf::Vertex(sf::Vector2f(destStation.pos.x, destStation.pos.y), color)   // End point (700, 500) with blue color
                };

                window.draw(line, 2, sf::Lines);
            }
        }

        /* PACKETS */
        for(auto& pair : linkManager.getLinks()) {
            auto& sourceStation = stations[pair.first];
            for(auto destId : pair.second) {
                auto& destStation = stations[destId];

                if(manager.getNetDevice(pair.first).isSending()
                        && manager.getNetDevice(destId).isReceiving()
                        && manager.getNetDevice(pair.first).getNextDevice()->getId() == destId) {

                    sf::CircleShape circle{5};
                    circle.setFillColor(sf::Color::White);
                    circle.setOrigin(circle.getRadius(), circle.getRadius());
                    
                    Vec3 offset = destStation.pos - sourceStation.pos;
                    Vec3 pos = destStation.pos - (offset *
                            (static_cast<float>(manager.getNetDevice(sourceStation.deviceId).getTxRemaining()) / NetDevice::TX_DELAY));
                    circle.setPosition(pos.x, pos.y);
                    window.draw(circle);

                }
            }
        }
        
        for(auto& station : stations) {
            sf::CircleShape circle(20.0f);
            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition({
                static_cast<float>(station.pos.x),
                static_cast<float>(station.pos.y)
            });
            auto& device = manager.getNetDevice(station.deviceId);
            if(!device.isSending() && !device.isReceiving()) {
                circle.setFillColor(sf::Color::White);
            }
            else if(device.isSending() && !device.isReceiving()) {
                circle.setFillColor(sf::Color::Green);
            }
            else if(!device.isSending() && device.isReceiving()) {
                circle.setFillColor(sf::Color::Yellow);
            }
            else {
                circle.setFillColor(sf::Color::Red);
            }

            window.draw(circle);
        }

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }

    return 0;
}
