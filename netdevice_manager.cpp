#include "netdevice_manager.h"
#include <vector>

void NetDeviceManager::genNetDevices(int count) {
    devices.clear();
    devices.reserve(count);
    for(int i = 0; i != count; ++i) {
        devices.emplace_back(i);
    }
}

std::vector<NetDevice>::iterator NetDeviceManager::begin() {
    return devices.begin();
}

std::vector<NetDevice>::iterator NetDeviceManager::end() {
    return devices.end();
}

const NetDevice& NetDeviceManager::getNetDevice(NetDeviceId id) const {
    return devices[id];
}

NetDevice& NetDeviceManager::getNetDevice(NetDeviceId id) {
    return devices[id];
}

void NetDeviceManager::updateDevices(Milliseconds delta) {
    for(auto& device : devices) {
        device.update(delta);
    }
}
