#include "netdevice_manager.h"
#include <vector>

void NetDeviceManager::load() {
    devices = loader->loadNetDevices();
}

const NetDevice& NetDeviceManager::getNetDevice(NetDeviceId id) const {
    return devices[id];
}

NetDevice& NetDeviceManager::getNetDevice(NetDeviceId id) {
    return devices[id];
}

std::vector<NetDevice>::iterator NetDeviceManager::begin() {
    return devices.begin();
}

std::vector<NetDevice>::iterator NetDeviceManager::end() {
    return devices.end();
}

void NetDeviceManager::queuePacket(Packet p, NetDeviceId source) {
    devices[source].queue(p);
}
