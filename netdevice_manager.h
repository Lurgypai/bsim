#pragma once
#include <vector>
#include <memory>

#include "netdevice.h"

class NetDeviceLoader {
public:
    virtual ~NetDeviceLoader() {};
    virtual std::vector<NetDevice> loadNetDevices() = 0;
    virtual std::unique_ptr<NetDeviceLoader> clone() = 0;
};

class NetDeviceManager {
public:
    template<typename Loader, typename... Args>
    Loader& setLoader(Args&&... args);

    void genNetDevices(int count);
    const NetDevice& getNetDevice(NetDeviceId id) const;
    NetDevice& getNetDevice(NetDeviceId id);

    std::vector<NetDevice>::iterator begin();
    std::vector<NetDevice>::iterator end();
private:
    std::unique_ptr<NetDeviceLoader> loader;
    std::vector<NetDevice> devices;
};

template<typename Loader, typename... Args>
Loader& NetDeviceManager::setLoader(Args&&... args) {
    loader = std::make_unique<Loader>(std::forward<Args>(args)...);
}
