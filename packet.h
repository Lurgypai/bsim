#pragma once

#include "types.h"

struct Packet {
    PacketId id;

    // ending target
    NetDeviceId target;
};
