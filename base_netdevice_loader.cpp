#include "base_netdevice_loader.h"
#include <memory>

BaseNetDeviceLoader::BaseNetDeviceLoader(int deviceCount_) :
    deviceCount{deviceCount_}
{}

std::vector<NetDevice> BaseNetDeviceLoader::loadNetDevices() {
    std::vector<NetDevice> devices;
    devices.reserve(deviceCount);
    for(int i = 0; i != deviceCount; ++i) {
        devices[i] = NetDevice{i};
    }
    return devices;
}

std::unique_ptr<NetDeviceLoader> BaseNetDeviceLoader::clone() {
    return std::make_unique<BaseNetDeviceLoader>(*this);
}
