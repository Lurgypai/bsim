#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "netdevice_manager.h"
#include "station_manager.h"
#include "link_manager.h"
#include "router.h"
#include "camera.h"

class Topology {
public:
    Topology();

    void update(Milliseconds delta);

    template<typename Loader, typename... Args>
    Loader& setNetDeviceLoader(Args&&... args);
    template<typename Loader, typename... Args>
    Loader& setStationLoader(Args&&... args);
    template<typename Loader, typename... Args>
    Loader& setLinkLoader(Args&&... args);
    template<typename RouterImpl, typename... Args>
    RouterImpl& setRouterImpl(Args&&... args);

    void loadAll();

    /* RENDERING */
    void prepareRendering();
    void draw(sf::RenderTarget& target);
    Camera cam;

    void queuePacket(Packet p, NetDeviceId source);
private:
    NetDeviceManager netDeviceManager;
    StationManager stationManager;
    LinkManager linkManager;
    Router router;

    /* RENDERING */
    struct AssignedCircle {
        NetDeviceId id;
        Vec3::Type z;
        sf::CircleShape shape;
    };

    std::list<AssignedCircle> stations;
};

#define MAKE_SET_LOADER(NAME, MANAGER) \
    template<typename Loader, typename... Args> \
    Loader& Topology::set##NAME##Loader(Args&&... args) { \
        return MANAGER.setLoader<Loader>(std::forward<Args>(args)...); \
    }

MAKE_SET_LOADER(NetDevice, netDeviceManager)
MAKE_SET_LOADER(Station, stationManager)
MAKE_SET_LOADER(Link, linkManager)

template<typename RouterImpl, typename... Args>
RouterImpl& Topology::setRouterImpl(Args&&... args) {
    return router.loadImpl<RouterImpl>(std::forward<Args>(args)...);
}
