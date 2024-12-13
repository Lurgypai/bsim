#pragma once

#include <memory>

#include "link_manager.h"

class SatgenpyLinkLoader : public LinkLoader {
public:
    SatgenpyLinkLoader(Milliseconds rate_);
    virtual ~SatgenpyLinkLoader() {};
    virtual LinkMap loadLinks() override;
    virtual void updateLinks(Milliseconds delta, LinkMap& links, StationManager& stations) override;
    virtual std::unique_ptr<LinkLoader> clone() override;
private:
    Milliseconds rate;
    Milliseconds remainder;
    Milliseconds elapsed;
};
