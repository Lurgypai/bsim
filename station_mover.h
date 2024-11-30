#pragma once

#include <memory>

#include "types.h"

class Station;

class StationMover {
public:
    virtual ~StationMover() {};
    virtual void moveStation(Station& station, Milliseconds delta) = 0;
    virtual std::unique_ptr<StationMover> clone() const = 0;
};
