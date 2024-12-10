#pragma once

#include "station.h"
#include <utility>
#include <vector>
#include <memory>

class StationLoader {
    virtual ~StationLoader() {};
    virtual std::vector<Station> loadStations() = 0;
    virtual std::unique_ptr<StationLoader> clone() = 0;
};

class StationManager {
public:
    template<typename Loader, typename... Args>
    Loader& setLoader(Args&&... args);

    void update(Milliseconds delta);

    const std::vector<Station>& getStations() const;
private:
    std::unique_ptr<StationLoader> loader;
    std::vector<Station> stations;
};

template<typename Loader, typename... Args>
Loader& StationManager::setLoader(Args&&... args) {
    loader = std::make_unique<Loader>(std::forward<Args>(args)...);
    return *loader;
}
