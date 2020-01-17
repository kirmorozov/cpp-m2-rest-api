cmake_minimum_required(VERSION 3.11)
project(mg_m2_api)

set(CMAKE_CXX_STANDARD 17)


include_directories(/usr/include)
include_directories(/usr/local/include)
include_directories(/usr/include/mysql-cppconn-8)
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

include_directories(model)
#include_directories(api)
#include_directories(impl)


add_executable(mg_m2_api main.cpp)
target_link_libraries(mg_m2_api pistache pthread mysqlcppconn8 sodium)
