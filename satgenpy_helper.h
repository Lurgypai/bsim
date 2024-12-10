#pragma once

#include <vector>
#include<string>

#include "link_manager.h"

namespace RouteHelper {
    std::vector<std::string> split_string(const std::string& line, const std::string& delimiter);
    LinkMap readRoutesFile(const std::string& fileName);
}
