#include "tle_mover.h"
#include "libsgp4/Vector.h"

#include <memory>

#include "station.h"

TleMover::TleMover(const std::string& tle1_, const std::string& tle2_) :
    now{2000, 1, 1},
    tle{tle1_, tle2_},
    model{tle}
{}

void TleMover::moveStation(Station& station, Milliseconds delta) {
    now = now.AddMicroseconds(delta * 1000);

    station.pos = getPosition();
}

std::unique_ptr<StationMover> TleMover::clone() const {
    return std::make_unique<TleMover>(*this);
}

static double MsToJulian(Milliseconds time) {
    return static_cast<double>(time) / 86400000.0;
}

Vec3 TleMover::getPosition() const {
    libsgp4::Eci eci = model.FindPosition(now.ToJ2000());
    libsgp4::Vector pos = eci.Position();

    double gmst = now.ToGreenwichSiderealTime();

    Vec3 itrf = {
        std::cos(gmst) * pos.x + std::sin(gmst) * pos.y,
        -std::sin(gmst) * pos.x + std::cos(gmst) * pos.y,
        pos.z
    };

    return itrf * 1000;
}

