#pragma once
#include <vector>
#include <string>

#include "station.h"

std::vector<Station> readGroundStations(const std::string& file);
std::vector<Station> readSatellites(const std::string& file);
