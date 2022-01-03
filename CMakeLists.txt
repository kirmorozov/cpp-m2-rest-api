cmake_minimum_required(VERSION 3.11)
project(mg_m2_api)

set(CMAKE_C_FLAGS "-Wall -c -O2 -fpic -o")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(target main.cpp)
target_link_libraries(target pthread stdc++fs crypto ssl)

include(3rdparty/zlib.cmake)
include(3rdparty/json.cmake)
include(3rdparty/inja.cmake)
include(3rdparty/hiredis.cmake)
include(3rdparty/pistache.cmake)
include(3rdparty/sodium.cmake)
include(3rdparty/mysqlx.cmake)
include(3rdparty/redis-plus-plus.cmake)

include_directories(model)
include_directories(3rdparty/includes)
include_directories(src)

set_target_properties(target PROPERTIES OUTPUT_NAME "mg_m2_api")