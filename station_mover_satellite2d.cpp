#include "station_mover_satellite2d.h"

#include <cmath>
#include <memory>

#include "station.h"

void StationMoverSatellite2d::moveStation(Station& station, Milliseconds delta) {
    time += delta;

    station.pos.x = std::cos(time * speed + offset) * radius;
    station.pos.y = std::sin(time * speed + offset) * radius;
};

std::unique_ptr<StationMover> StationMoverSatellite2d::clone() const {
    return std::make_unique<StationMoverSatellite2d>(*this);
}
