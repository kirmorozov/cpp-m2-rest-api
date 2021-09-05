
message(STATUS "Checking for json: Please wait")
include(ExternalProject)

ExternalProject_Add(json
        PREFIX "${CMAKE_BINARY_DIR}/lib"
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG "v3.10.2"
        CMAKE_ARGS
        -DJSON_CI=ON
        -DJSON_BuildTests=OFF
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib/installed
        )

target_include_directories(target PUBLIC ${CMAKE_BINARY_DIR}/lib/installed/include)

add_dependencies(target json)

message(STATUS "Checking for json: DONE")

