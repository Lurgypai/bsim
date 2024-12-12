#pragma once

#include <unordered_map>

#include "router.h"

using RouteMap = std::unordered_map<NetDeviceId, std::unordered_map<NetDeviceId, NetDeviceId>>;

class SatgenpyRouterImpl : public RouterImpl {
public:
    SatgenpyRouterImpl(int rate_);
    virtual ~SatgenpyRouterImpl() {};
private:
    virtual NetDeviceId findNext(NetDeviceId curId, NetDeviceId destId) override;
    virtual void update(Milliseconds delta, StationManager& stations, LinkManager& links) override;

    RouteMap routes;

    int rate;
    int remainder;
    int elapsed;
};
