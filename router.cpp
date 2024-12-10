#include "router.h"

void Router::update(Milliseconds delta, StationManager& stations, LinkManager& links) {
    impl->update(delta, stations, links);
}

NetDeviceId Router::findNext(NetDeviceId curId, NetDeviceId destId) {
    return impl->findNext(curId, destId);
}

