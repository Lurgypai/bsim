#include "station.h"
#include "station_mover.h"
#include <memory>

Station::Station(NetDeviceId deviceId_) :
    deviceId{deviceId_},
    pos{0, 0, 0},
    tag{},
    mover{nullptr}
{}

Station::Station(const Station& other) :
    deviceId{other.deviceId},
    pos{other.pos},
    tag{other.tag},
    mover{nullptr}
{
    if(other.mover) mover = other.mover->clone();
}

Station& Station::operator=(const Station& other) {
    deviceId = other.deviceId;
    pos = other.pos;
    tag = other.tag;
    if(other.mover) mover = other.mover->clone();
    else mover = nullptr;
    
    return *this;
}

void Station::update(Milliseconds delta) {
    if(!mover) return;
    mover->moveStation(*this, delta);
}
