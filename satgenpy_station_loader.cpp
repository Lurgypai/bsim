#include "satgenpy_station_loader.h"

#include <memory>
#include <iostream>

#include "readers.h"


std::vector<Station> SatgenpyStationLoader::loadStations() {
    auto gs = readGroundStations("ground_stations.txt");
    auto s = readSatellites("tles.txt");

    groundStationCount = gs.size();
    satelliteCount = s.size();

    std::cout << "Loaded " << gs.size() << " ground stations and " << s.size() << " satellites.\n";

    s.reserve(s.size() + gs.size());
    s.insert(s.end(), gs.begin(), gs.end());
    return s;
}

std::unique_ptr<StationLoader> SatgenpyStationLoader::clone() {
    return std::make_unique<SatgenpyStationLoader>(*this);
}

int SatgenpyStationLoader::getGroundStationCount() const {
    return groundStationCount;
}

int SatgenpyStationLoader::getSatelliteCount() const {
    return satelliteCount;
}
