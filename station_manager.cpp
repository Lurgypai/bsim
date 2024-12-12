#include "station_manager.h"
#include <vector>

void StationManager::load() {
    stations = loader->loadStations();
}

void StationManager::update(Milliseconds delta) {
    for(auto& station : stations) {
        station.update(delta);
    }
}

const std::vector<Station>& StationManager::getStations() const {
    return stations;
}
