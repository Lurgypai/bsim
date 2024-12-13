#include "link_manager.h"

LinkManager::LinkManager(const LinkManager& other) :
    links{other.links},
    loader{other.loader->clone()}
{}

LinkManager& LinkManager::operator=(const LinkManager& other) {
    links = other.links;
    loader = other.loader->clone();
    return *this;
}

void LinkManager::load() {
    links = loader->loadLinks();
}

void LinkManager::updateLinks(Milliseconds delta, StationManager& station) {
    loader->updateLinks(delta, links, station);
}

const Link& LinkManager::getLink(NetDeviceId source, NetDeviceId dest) const {
    return links.at(source).at(dest);
}

Link& LinkManager::getLink(NetDeviceId source, NetDeviceId dest) {
    return links.at(source).at(dest);
}

const LinkMap& LinkManager::getLinks() const {
    return links;
}
