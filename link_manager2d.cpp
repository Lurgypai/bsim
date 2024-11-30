#include "link_manager2d.h"
#include <unordered_map>
#include <vector>

void LinkManager2d::update(const std::vector<Station>& stations) {
    for(const auto& station : stations) {
        links[station.deviceId].clear();
        for(const auto& other : stations) {
            if(other.deviceId == station.deviceId) continue;
            if(station.pos.dist(other.pos) < 130.f) {
                links[station.deviceId].push_back(other.deviceId);
            }
        }
    }
}

const std::unordered_map<NetDeviceId, std::vector<NetDeviceId>>& LinkManager2d::getLinks() const {
    return links;
}
