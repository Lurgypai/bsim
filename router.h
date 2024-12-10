#pragma once
#include <memory>

#include "netdevice.h"
#include "station_manager.h"
#include "link_manager.h"

class RouterImpl;

class Router {
public:
    Router() = default;

    Router(const Router& other) = delete;
    Router& operator=(const Router& other) = delete;

    void update(Milliseconds delta, StationManager& stations, LinkManager& links);

    NetDeviceId findNext(NetDeviceId curId, NetDeviceId destId);
    template <typename Impl, typename... Args>
    void loadImpl(Args&&... args);
private:
    std::unique_ptr<RouterImpl> impl;
};

template<typename Impl, typename... Args>
void Router::loadImpl(Args&&... args) {
    impl = std::make_unique<Impl>(std::forward<Args>(args)...);
}

class RouterImpl {
    friend class Router;
public:
    virtual ~RouterImpl() {}
private:
    virtual NetDeviceId findNext(NetDeviceId curId, NetDeviceId destId) = 0;
    virtual void update(Milliseconds delta, StationManager& stations, LinkManager& links) = 0;
};
