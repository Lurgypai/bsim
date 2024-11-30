cmake_minimum_required(VERSION 3.20)

project(badsim)

include_directories("./sgp4-ins/include")
link_directories("./sgp4-ins/lib")

add_library(bsim "router.cpp" "netdevice.cpp" "netdevice_manager.cpp" "router.cpp" "station.cpp" "vec3.cpp")

add_executable(bsim_test_2d "bsim_test_2d.cpp")
add_executable(bsim_test_2d_moving "bsim_test_2d_moving.cpp" "station_mover_satellite2d.cpp" "link_manager2d.cpp")
add_executable(bsim_test_3d "bsim_test_3d.cpp" "satellite_renderer.cpp" "camera.cpp")
add_executable(bsim_3d "bsim_3d.cpp" "satellite_renderer.cpp" "camera.cpp" "readers.cpp" "tle_mover.cpp" "link_manager_3d.cpp")

target_link_libraries(bsim_test_2d bsim)
target_link_libraries(bsim_test_2d_moving bsim sfml-system sfml-window sfml-graphics)
target_link_libraries(bsim_test_3d bsim sfml-system sfml-window sfml-graphics)
target_link_libraries(bsim_3d bsim sfml-system sfml-window sfml-graphics sgp4s)
