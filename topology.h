#pragma once

#include "netdevice_manager.h"
#include "station_manager.h"
#include "link_manager.h"
#include "router.h"

class Topology {
public:
    void update(Milliseconds delta);

    template<typename Loader, typename... Args>
    void setNetDeviceLoader(Args&&... args);
    template<typename Loader, typename... Args>
    void setStationLoader(Args&&... args);
    template<typename Loader, typename... Args>
    void setLinkLoader(Args&&... args);
    template<typename RouterImpl, typename... Args>
    void setRouterImpl(Args&&... args);
private:
    NetDeviceManager netDeviceManager;
    StationManager stationManager;
    LinkManager linkManager;
    Router router;
};
