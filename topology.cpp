#include "topology.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

Topology::Topology() :
    cam{},
    netDeviceManager{},
    stationManager{},
    linkManager{},
    router{},
    stations{}
{
    stations.emplace_back(
        AssignedCircle{-1, 0., sf::CircleShape{300}}
    );
    auto& earth = stations.back();
    earth.shape.setFillColor(sf::Color::Blue);
    earth.shape.setOrigin(earth.shape.getRadius(), earth.shape.getRadius());
}

// try to start sending data if there is data available
static void tryBeginSend(NetDevice& device,
                         LinkManager& links,
                         NetDeviceManager& netDevices,
                         Router& router) {

    // go until we hit the max concurrent links active
    // this will cause devices that are update earlier to have sending precedence.
    while(device.hasPackets() && !device.isBusy()) {
        const Packet& nextPacket = device.peekNextPacket();
        NetDeviceId nextDeviceId = router.findNext(device.getId(), nextPacket.target);

        // get the link this will send over
        // check if its busy
        Link& upLink = links.getLink(device.getId(), nextDeviceId);
        if(upLink.busy) return;

        Link& downLink = links.getLink(nextDeviceId, device.getId());
        if(downLink.busy) return;

        // get the receiving net device, and increment its active links
        NetDevice& nextDevice = netDevices.getNetDevice(nextDeviceId);
        device.addActiveLink();
        nextDevice.addActiveLink();

        // pop the packet and set up an in flight entry to track its progress
        device.putNextInFlight(nextDevice.getId());

        // set the uplink and downlink as active
        upLink.busy = true;
        downLink.busy = true;
    }
}

static void continueSend(NetDevice& device, Milliseconds delta) {
    // decrement time of each in flight packet
    device.continueSend(delta);
}

static void endSend(NetDevice& device,
                    NetDeviceManager& devices,
                    LinkManager& links) {
    // for each in flight packet
    auto& inFlightPackets = device.getInFlightPackets();
    for(auto iter = inFlightPackets.begin(); iter != inFlightPackets.end();) {
        if(iter->txRemaining != 0) {
            ++iter;
            continue;
        }

        NetDeviceId destId = iter->dest;
        auto& destDevice = devices.getNetDevice(destId);

        // set links as available
        Link& upLink = links.getLink(device.getId(), destId);
        Link& downLink = links.getLink(destId, device.getId());
        upLink.busy = false;
        downLink.busy = false;

        // derement active links
        device.removeActiveLink();
        destDevice.removeActiveLink();

        // queue on destination device (stops the packet if destination)
        destDevice.queue(iter->packet);

        // remove from in flight
        iter = inFlightPackets.erase(iter);
    }
}   

void Topology::update(Milliseconds delta) {
    stationManager.update(delta);
    linkManager.updateLinks(delta, stationManager);
    router.update(delta, stationManager, linkManager);
    
    for(auto& netDevice : netDeviceManager) {
        endSend(netDevice, netDeviceManager, linkManager);
    }
    for(auto& netDevice : netDeviceManager) {
        tryBeginSend(netDevice, linkManager, netDeviceManager, router);
    }
    for(auto& netDevice : netDeviceManager) {
        continueSend(netDevice, delta);
    }
}

void Topology::loadAll() {
    netDeviceManager.load();
    stationManager.load();
    linkManager.load();
}

void Topology::prepareRendering() {
    const auto& stationsToRender = stationManager.getStations();

    for(int id = 0; id != stationsToRender.size(); ++id) {
        stations.emplace_back(
                AssignedCircle{id, 0., sf::CircleShape{5}}
        );
        auto& station = stations.back();
        station.shape.setOrigin(station.shape.getRadius(), station.shape.getRadius());

        if(stationsToRender[id].tag == "ground") station.shape.setFillColor(sf::Color::Green);
        else station.shape.setFillColor(sf::Color::Red);
    }
}

void Topology::draw(sf::RenderTarget& target) {
    /* draw stations */
    for(auto& circle : stations) {
        if(circle.id < 0) continue; // earth
        auto& station = stationManager.getStations()[circle.id];
        //std::cout << station.pos.x << ", " << station.pos.y << ", " << station.pos.z << '\n';

        Vec3 pos = cam.project(station.pos);
        circle.z = pos.z;
        circle.shape.setPosition(pos.x, pos.y);
    }
    stations.sort(
        [](const AssignedCircle& a, const AssignedCircle& b) -> bool {
        return a.z < b.z;
    });

    for(auto& station : stations) {
        target.draw(station.shape);
    }

    for(const auto& [sourceId, route] : linkManager.getLinks()) {
        const Station& source = stationManager.getStations()[sourceId];
        Vec3 p1 = cam.project(source.pos);
        if(p1.z < 0) continue;

        for(const auto& [destId, link] : route) {
            const Station& dest = stationManager.getStations()[destId];
            Vec3 p2 = cam.project(dest.pos);
            if(p2.z < 0) continue;

            auto color = sf::Color::Red;
            if(link.busy) color = sf::Color::White;
            else if(source.tag == "ground" || dest.tag == "ground") color = sf::Color::Green;

            sf::Vertex line[]{
                sf::Vertex{sf::Vector2f{static_cast<float>(p1.x), static_cast<float>(p1.y)}, color},
                sf::Vertex{sf::Vector2f{static_cast<float>(p2.x), static_cast<float>(p2.y)}, color}
            };

            target.draw(line, 2, sf::Lines);
        }
    }
}

void Topology::queuePacket(Packet p, NetDeviceId source) {
    netDeviceManager.queuePacket(p, source);
}
