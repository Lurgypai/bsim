#pragma once
#include <unordered_map> 
#include <vector>

#include "netdevice.h"

class NetDeviceManager {
public:
    void genNetDevices(int count);
    std::vector<NetDevice>::iterator begin();
    std::vector<NetDevice>::iterator end();
    const NetDevice& getNetDevice(NetDeviceId id) const;
    NetDevice& getNetDevice(NetDeviceId id);
    void updateDevices(Milliseconds delta);
private:
    std::vector<NetDevice> devices;
};
