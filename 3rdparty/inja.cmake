
message(STATUS "Checking for inja: Please wait")
include(ExternalProject)

ExternalProject_Add(inja
        PREFIX "${CMAKE_BINARY_DIR}/lib"
        GIT_REPOSITORY "https://github.com/pantor/inja.git"
        GIT_TAG "v3.3.0"
        CMAKE_ARGS
        -DINJA_USE_EMBEDDED_JSON=OFF
        -DINJA_BUILD_TESTS=OFF
        -DBUILD_BENCHMARK=OFF
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib/installed
        )

target_include_directories(target PUBLIC ${CMAKE_BINARY_DIR}/lib/installed/include)

add_dependencies(target inja)

message(STATUS "Checking for inja: DONE")

