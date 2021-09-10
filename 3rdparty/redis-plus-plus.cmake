
message(STATUS "Checking for redis-plus-plus: Please wait")
include(ExternalProject)

ExternalProject_Add(redis-plus-plus
        PREFIX "${CMAKE_BINARY_DIR}/lib"
        GIT_REPOSITORY "https://github.com/sewenew/redis-plus-plus.git"
        GIT_TAG "1.3.1"
        CMAKE_ARGS
            -DREDIS_PLUS_PLUS_CXX_STANDARD=17
            -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib/installed
            -DREDIS_PLUS_PLUS_BUILD_TEST=OFF
        )

#include_directories(${CMAKE_BINARY_DIR}/lib/installed/include)
target_include_directories(target PUBLIC ${CMAKE_BINARY_DIR}/lib/installed/include)
target_link_directories(target PUBLIC ${CMAKE_BINARY_DIR}/lib/installed/lib)
target_link_libraries(target redis++)

add_dependencies(target redis-plus-plus)

message(STATUS "Checking for redis-plus-plus: DONE")

