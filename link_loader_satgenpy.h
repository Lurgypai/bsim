#pragma once

#include "link_manager.h"

class LinkLoaderSatgenpy : public LinkLoader {
    virtual ~LinkLoaderSatgenpy() {};
    virtual LinkMap loadLinks() override;
    virtual std::unique_ptr<LinkLoader> clone() override;
};
