#pragma once

#include <unordered_map>
#include <memory>

#include "types.h"

struct Link {
    NetDeviceId source;
    NetDeviceId dest;
    bool busy;
};

using LinkMap = std::unordered_map<NetDeviceId, std::unordered_map<NetDeviceId, Link>>;

class LinkLoader {
public:
    virtual ~LinkLoader() {};
    virtual LinkMap loadLinks() = 0;
    virtual void updateLinks(Milliseconds delta, LinkMap& links) = 0;
    virtual std::unique_ptr<LinkLoader> clone() = 0;
};


class LinkManager {
public:
    LinkManager() = default;

    LinkManager(const LinkManager& other);
    LinkManager& operator=(const LinkManager& other);

    template<typename Loader, typename... Args>
    Loader& setLoader(Args&&... args);

    void load();
    void updateLinks(Milliseconds delta);
    const Link& getLink(NetDeviceId source, NetDeviceId target) const;
    Link& getLink(NetDeviceId source, NetDeviceId target);
private:
    std::unique_ptr<LinkLoader> loader;
    LinkMap links;
};

template<typename Loader, typename... Args>
Loader& LinkManager::setLoader(Args&&... args) {
    loader = std::make_unique<Loader>(std::forward<Args>(args)...);
    return *static_cast<Loader*>(loader.get());
}
