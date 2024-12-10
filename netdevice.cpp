#include "netdevice.h"
#include "router.h"
#include <exception>

NetDevice::NetDevice(NetDeviceId id_) :
    id{id_},
    tx_remaining{0},
    packets{},
    curNextDevice{nullptr},
    receiving{},
    isSending_{false},
    isReceiving_{false},
    receivedCount{0}
{}

void NetDevice::queue(const Packet& packet) {
    packets.push_back(packet);
}

void NetDevice::update(Milliseconds delta, Router& router) {

    if(tx_remaining == 0) endSend();
    if(!isSending_) beginSend(router);
    if(isSending_) continueSend(delta);
}

bool NetDevice::isSending() const {
    return isSending_;
}

bool NetDevice::isReceiving() const {
    return isReceiving_;
}

NetDeviceId NetDevice::getId() const {
    return id;
}

const NetDevice* NetDevice::getNextDevice() const {
    return curNextDevice;
}

int NetDevice::getReceivedCount() const {
    return receivedCount;
}

void NetDevice::continueSend(Milliseconds delta) {
    if(tx_remaining > delta) tx_remaining -= delta;
    else tx_remaining = 0;
}

void NetDevice::beginSend(Router& router) {
    // nothing to send
    if (packets.empty()) return;

    Packet nextPacket = packets.front();
    curNextDevice = router.findNext(id, nextPacket.target);

    // route currently doesn't exist
    if(curNextDevice == nullptr) return;

    // we've been routed to ourselves, thats no good
    if(curNextDevice->id == id) throw std::exception{};

    // target device is busy
    if(curNextDevice->isReceiving_) return;

    packets.pop_front();
    tx_remaining = TX_DELAY;
    isSending_ = true;

    curNextDevice->receiving = nextPacket;
    curNextDevice->beginReceive();
}

void NetDevice::endSend() {
    if(!isSending_ || !curNextDevice) return;
    isSending_ = false;
    curNextDevice->endReceive();
}

void NetDevice::beginReceive() {
    isReceiving_ = true;
}

void NetDevice::endReceive() {
    if(receiving.target != id) queue(receiving);
    isReceiving_ = false;
    ++receivedCount;
}

Milliseconds NetDevice::getTxRemaining() const {
    return tx_remaining;
}

Router* NetDevice::router = nullptr;
