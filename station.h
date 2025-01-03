#pragma once

#include <memory>

#include "types.h"
#include "vec3.h"

#include "station_mover.h"

class Station {
public:
    Station() = default;
    Station(NetDeviceId deviceId_);

    Station(const Station& other);
    Station& operator=(const Station& other);

    template<typename Mover, typename... Args>
    Mover& loadMover(Args&&... args);

    void update(Milliseconds delta);

    Vec3 pos;
    NetDeviceId deviceId;
private:
    std::unique_ptr<StationMover> mover;
};

template<typename Mover, typename... Args>
Mover& Station::loadMover(Args&&... args) {
    mover = std::make_unique<Mover>(std::forward<Args>(args)...);
    return *dynamic_cast<Mover*>(mover.get());
}
