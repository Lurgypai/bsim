#include "netdevice.h"
#include <stdexcept>

NetDevice::NetDevice(NetDeviceId id_) :
    id{id_},
    packets{},
    inFlightPackets{},
    receivedCount{0},
    curActiveLinks{0},
    maxActiveLinks{0}
{}

void NetDevice::queue(const Packet& packet) {
    if(packet.target == id) {
        ++receivedCount;
        return;
    }

    packets.push_back(packet);
}

bool NetDevice::hasPackets() const {
    return packets.size() > 0;
}

bool NetDevice::isBusy() const {
    return curActiveLinks == maxActiveLinks;
}

const Packet& NetDevice::peekNextPacket() const {
    return packets.back();
}

void NetDevice::addActiveLink() {
    ++curActiveLinks;
}

void NetDevice::removeActiveLink() {
    if(curActiveLinks == 0) throw std::runtime_error{"Tried to remove an active link with no links"};
    --curActiveLinks;
}

void NetDevice::putNextInFlight(NetDeviceId dest) {
    inFlightPackets.push_back(InFlightPacket{
        packets.back(),
        dest,
        TX_DELAY
    });

    packets.pop_back();
}

std::vector<InFlightPacket>& NetDevice::getInFlightPackets() {
    return inFlightPackets;
}


NetDeviceId NetDevice::getId() const {
    return id;
}

int NetDevice::getReceivedCount() const {
    return receivedCount;
}

void NetDevice::continueSend(Milliseconds delta) {
    for(auto& packet : inFlightPackets) {
        auto& txRemaining = packet.txRemaining;
        if(txRemaining > delta) txRemaining -= delta;
        else txRemaining = 0;
    }
}
