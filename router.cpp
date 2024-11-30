#include "router.h"

NetDevice* Router::findNext(NetDeviceId curId, NetDeviceId destId) {
    return impl->findNext(curId, destId);
}

RouterImpl* Router::getImpl() {
    return impl.get();
}
