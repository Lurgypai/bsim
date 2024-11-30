#pragma once

#include <unordered_map>
#include <vector>

#include "types.h"
#include "station.h"

class LinkManager2d {
public:
    void update(const std::vector<Station>& stations);
    const std::unordered_map<NetDeviceId, std::vector<NetDeviceId>>& getLinks() const;
private:
    std::unordered_map<NetDeviceId, std::vector<NetDeviceId>> links;
};
