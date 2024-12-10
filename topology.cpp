#pragma once
#include "topology.h"

// try to start sending data if there is data available
static void tryBeginSend(NetDevice& device,
                         LinkManager& links,
                         NetDeviceManager& netDevices,
                         Router& router) {
    if(!device.hasPackets()) return;
    if(device.isBusy()) return;

    // for each next packet
    //      attempt to send
    //      if sending
    //          add to in flight packets

    Packet* nextPacket = device.peekNextPacket();
    NetDeviceId nextDeviceId = router.findNext(device.getId(), nextPacket->target);

    // get the link this will send over
    // check if its busy
    Link& upLink = links.getLink(device.getId(), nextDeviceId);
    if(upLink.busy) return;

    Link& downLink = links.getLink(nextDeviceId, device.getId());
    if(downLink.busy) return;

    // get the receiving net device, and increment its active links
    NetDevice& nextDevice = netDevices.getNetDevice(nextDeviceId);
    netDevice.addActiveLink();

    // set the uplink and downlink as active, and set their packet handle
    upLink.busy = true;
    upLink.packetHandle = nextPacket;

    downLink.busy = true;
    downLink.packetHandle = nextPacket;
}

static void continueSend(NetDevice& device, Milliseconds delta) {
    device.continueSend(delta);
}

static void endSend(NetDevice& device) {
    // for each in flight packet
    //      if its done sending
    //          close the link
    //          decrement active links
    //          remove from in flight packets
    //          queue the packet on the receiving net device

}

void Topology::update(Milliseconds delta) {
    stationManager.update(delta);
    linkManager.updateLinks(delta);
    router.update(delta, stationManager, linkManager);
    
    // update network

    for(auto& netDevice : netDeviceManager) {
        tryBeginSend(netDevice, linkManager, netDeviceManager, router);
        continueSend(netDevice);
        endSend(netDevice);
    }
}

// net device needs to store in flight packets
//  in flight
//      packet being sent
//      target network device
//      link being used <- can be retrieved from the source and des:wq
