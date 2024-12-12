#pragma once

#include "link_manager.h"
#include <memory>

class SatgenpyLinkLoader : public LinkLoader {
public:
    SatgenpyLinkLoader(int rate_);
    virtual ~SatgenpyLinkLoader() {};
    virtual LinkMap loadLinks() override;
    virtual void updateLinks(Milliseconds delta, LinkMap& links) override;
    virtual std::unique_ptr<LinkLoader> clone() override;
private:
    int rate;
    int remainder;
    int elapsed;
};
