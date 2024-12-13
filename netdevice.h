#pragma once

#include <deque>
#include <vector>

#include "packet.h"

class Router;
class NetDevice;

// CANT SEND AND RECEIVE SIMULTANEOUSLY, HMM
struct InFlightPacket {
    Packet packet;
    NetDeviceId dest;
    Milliseconds txRemaining;
};

// A thing that sends and receives packets
class NetDevice {
public:
    NetDevice() = default;
    NetDevice(NetDeviceId id);

    NetDevice(const NetDevice& other) = default;
    NetDevice& operator=(const NetDevice& other) = default;

    static constexpr Milliseconds TX_DELAY = 50;

    // Sends/Queues a packet
    void queue(const Packet& packet);

    bool hasPackets() const;
    bool isBusy() const;

    const Packet& peekNextPacket() const;

    void addActiveLink();
    void removeActiveLink();

    void putNextInFlight(NetDeviceId dest);
    std::vector<InFlightPacket>& getInFlightPackets();

    int getReceivedCount() const;
    NetDeviceId getId() const;

    void continueSend(Milliseconds delta);
    int maxActiveLinks;
private:

    NetDeviceId id;

    std::deque<Packet> packets;

    // may need to switch to a non-contiguous so that removal later is more efficient
    std::vector<InFlightPacket> inFlightPackets;

    int receivedCount;
    int curActiveLinks;
};
