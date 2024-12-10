#include "link_loader_satgenpy.h"

#include "satgenpy_helper.h"

LinkMap LinkLoaderSatgenpy::loadLinks() {
    return RouteHelper::readRoutesFile("routes/fstate_0.txt");
}

std::unique_ptr<LinkLoader> LinkLoaderSatgenpy::clone() {
    return std::make_unique<LinkLoaderSatgenpy>(*this);
}
