#pragma once

#include "station_mover.h"

class StationMoverSatellite2d : public StationMover {
public:
    virtual ~StationMoverSatellite2d() {};

    virtual void moveStation(Station& station, Milliseconds delta) override;
    virtual std::unique_ptr<StationMover> clone() const override;

    float radius;
    float speed;
    float offset;
private:
    Milliseconds time;
};
