include_directories(/usr/local/include)
include_directories(/usr/include/mysql-cppconn-8)

find_library(MYSQLX_LIB mysqlcppconn8)
target_link_libraries(target ${MYSQLX_LIB})

