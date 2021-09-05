

find_path(PISTACHE_HEADER pistache)
target_include_directories(target PUBLIC ${PISTACHE_HEADER})

find_library(PISTACHE_LIB pistache)
target_link_libraries(target ${PISTACHE_LIB})