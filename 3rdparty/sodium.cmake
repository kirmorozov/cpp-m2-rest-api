

find_path(SODIUM_HEADER sodium)
target_include_directories(target PUBLIC ${SODIUM_HEADER})

find_library(SODIUM_LIB sodium)
target_link_libraries(target ${SODIUM_LIB})