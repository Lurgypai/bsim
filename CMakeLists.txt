cmake_minimum_required(VERSION 3.20)

project(badsim)

include_directories("./sgp4-ins/include")
link_directories("./sgp4-ins/lib")

add_library(bsim
    "topology.cpp"
    "camera.cpp"
    "router.cpp"
    "link_manager.cpp"
    "netdevice.cpp"
    "netdevice_manager.cpp"
    "station.cpp"
    "station_manager.cpp"
    "vec3.cpp"
)

add_executable(bsim_3d
    "bsim_3d.cpp"
    "base_netdevice_loader.cpp"
    "satgenpy_station_loader.cpp"
    "satgenpy_link_loader.cpp"
    "satgenpy_router_impl.cpp"
    "readers.cpp"
    "tle_mover.cpp"
    "satgenpy_helper.cpp"
)

target_link_libraries(bsim_3d bsim sfml-system sfml-window sfml-graphics sgp4s)
