#pragma once

#include "netdevice_manager.h"
#include <memory>
#include <vector>

class BaseNetDeviceLoader : public NetDeviceLoader {
public:
    BaseNetDeviceLoader(int deviceCount_);
    int deviceCount;

    virtual ~BaseNetDeviceLoader() {};
    virtual std::vector<NetDevice> loadNetDevices() override;
    virtual std::unique_ptr<NetDeviceLoader> clone() override;
};
