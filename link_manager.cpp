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

void LinkManager::loadLinks() {
    links = loader->loadLinks();
}

void LinkManager::updateLinks(Milliseconds delta) {
    loader->updateLinks(delta, links);
}

const Link& LinkManager::getLink(NetDeviceId source, NetDeviceId dest) const {
    return links.at(source).at(dest);
}
