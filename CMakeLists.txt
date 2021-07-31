cmake_minimum_required(VERSION 3.11)
project(mg_m2_api)

set(CMAKE_C_FLAGS "-Wall -c -O2 -fpic -o")
set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/include)
include_directories(/usr/local/include)
include_directories(/usr/include/mysql-cppconn-8)



link_directories(/usr/lib64)
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)
link_directories(/usr/lib64/php/modules)

include_directories(model)
include_directories(3rdparty/includes)
#include_directories(api)
#include_directories(impl)


add_executable(mg_m2_api main.cpp)
target_link_libraries(mg_m2_api pistache pthread mysqlcppconn8 sodium stdc++fs crypto ssl)
# target_link_libraries(mg_m2_api crypt   resolv crypt stdc++ z pcre rt m dl  xml2 z lzma m dl gssapi_krb5 krb5 k5crypto com_err ssl crypto crypt crypt)
