#pragma once

#include <memory>
#include <string>

#include "libsgp4/DateTime.h"
#include "libsgp4/Tle.h"
#include "libsgp4/SGP4.h"

#include "vec3.h"
#include "station_mover.h"

class TleMover : public StationMover {
public:
    TleMover(const std::string& tle1_, const std::string& tle2_);
    virtual ~TleMover() {};
    virtual void moveStation(Station& station, Milliseconds delta) override;
    virtual std::unique_ptr<StationMover> clone() const override;

    Vec3 getPosition() const;
private:
    libsgp4::DateTime now;
    libsgp4::Tle tle;
    libsgp4::SGP4 model;
};
