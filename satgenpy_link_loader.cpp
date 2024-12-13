#include "satgenpy_link_loader.h"
#include <memory>
#include <string>

#include "station_manager.h"
#include "satgenpy_helper.h"

SatgenpyLinkLoader::SatgenpyLinkLoader(Milliseconds rate_) :
    rate{rate_},
    remainder{0},
    elapsed{0}
{}

LinkMap SatgenpyLinkLoader::loadLinks() {
    auto routeSteps = RouteHelper::readRoutesFile("routes/fstate_0.txt");
    LinkMap links;
    for(const auto& step : routeSteps) {
        links[step.cur][step.next] = Link{
            step.cur,
            step.next,
            false
        };
    }
    return links;
}

void SatgenpyLinkLoader::updateLinks(Milliseconds delta, LinkMap& links, StationManager& stations) {
    remainder += delta;
    while(remainder >= rate) {
        remainder -= rate;
        elapsed += rate;

        std::string targetFile{"routes/fstate_" + std::to_string(elapsed) + "000000.txt"};
        auto routeSteps = RouteHelper::readRoutesFile(targetFile);
        for(const auto& step : routeSteps) {
            links[step.cur][step.next] = Link{
                step.cur,
                step.next,
                false
            };
        }
    }
}

std::unique_ptr<LinkLoader> SatgenpyLinkLoader::clone() {
    return std::make_unique<SatgenpyLinkLoader>(*this);
}
