#pragma once

#include <vector>
#include <string>

#include "types.h"

namespace RouteHelper {
    struct RouteStep {
        NetDeviceId cur;
        NetDeviceId dest;
        NetDeviceId next;
    };

    std::vector<std::string> split_string(const std::string& line, const std::string& delimiter);
    std::vector<RouteStep>  readRoutesFile(const std::string& fileName);
}
