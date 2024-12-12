#include "satgenpy_router_impl.h"

#include "satgenpy_helper.h"

SatgenpyRouterImpl::SatgenpyRouterImpl(int rate_) :
    rate{rate_},
    remainder{0},
    elapsed{0}
{
    std::string targetFile{"routes/fstate_0.txt"};
    auto routeSteps = RouteHelper::readRoutesFile(targetFile);
    for(const auto& step : routeSteps) {
        routes[step.cur][step.dest] = step.next;
    }
}

NetDeviceId SatgenpyRouterImpl::findNext(NetDeviceId curId, NetDeviceId destId) {
    return routes.at(curId).at(destId);
}

void SatgenpyRouterImpl::update(Milliseconds delta, StationManager& stations, LinkManager& links) {
    remainder += delta;
    while(remainder >= rate) {
        remainder -= rate;
        elapsed += rate;
        std::string targetFile{"routes/fstate_" + std::to_string(elapsed) + "000000.txt"};
        auto routeSteps = RouteHelper::readRoutesFile(targetFile);
        for(const auto& step : routeSteps) {
            routes[step.cur][step.dest] = step.next;
        }
    }
}
