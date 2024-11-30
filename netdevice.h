#pragma once

#include <deque>

#include "packet.h"

class Router;

// CANT SEND AND RECEIVE SIMULTANEOUSLY, HMM

// A thing that sends and receives packets
class NetDevice {
public:
    NetDevice() = default;
    NetDevice(NetDeviceId id);

    NetDevice(const NetDevice& other) = default;
    NetDevice& operator=(const NetDevice& other) = default;

    static constexpr Milliseconds TX_DELAY = 50;
    static Router* router;

    // Sends/Queues a packet
    void queue(const Packet& packet);

    // Sends queued packets
    void update(Milliseconds delta);

    bool isSending() const;
    bool isReceiving() const;
    int getReceivedCount() const;
    NetDeviceId getId() const;

    const NetDevice* getNextDevice() const;

    Milliseconds getTxRemaining() const;
private:
    void continueSend(Milliseconds delta);
    void beginSend();
    void endSend();
    void beginReceive();
    void endReceive();

    NetDeviceId id;

    // remaining time till transfer is complete
    Milliseconds tx_remaining;

    std::deque<Packet> packets;

    NetDevice* curNextDevice;

    // current packet being sent/received
    Packet receiving;

    bool isSending_;
    bool isReceiving_;

    int receivedCount;
};
