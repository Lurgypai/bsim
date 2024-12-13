#pragma once

#include "station_manager.h"
#include <memory>

class SatgenpyStationLoader : public StationLoader {
public:
    SatgenpyStationLoader();

    virtual ~SatgenpyStationLoader() {};
    virtual std::vector<Station> loadStations() override;
    virtual std::unique_ptr<StationLoader> clone() override;

    int getGroundStationCount() const;
    int getSatelliteCount() const;
private:
    size_t groundStationCount;
    size_t satelliteCount;
};
