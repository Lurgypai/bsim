#pragma once

#include "station_manager.h"
#include <memory>

class SatgenpyStationLoader : public StationLoader {
public:
    virtual ~SatgenpyStationLoader() {};
    virtual std::vector<Station> loadStations() override;
    virtual std::unique_ptr<StationLoader> clone() override;

    int getGroundStationCount() const;
    int getSatelliteCount() const;
private:
    int groundStationCount;
    int satelliteCount;
};
