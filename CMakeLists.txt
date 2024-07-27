cmake_minimum_required(VERSION 3.22)
project(tlvjsonc VERSION 0.0.1
        DESCRIPTION "Type Length Value(JSON) protocol base on TCP"
        LANGUAGES C)


find_library(LIBEVENT2_CORE event_core)
find_library(cJSON cjson)

# The compiled library code is here
add_subdirectory(src)

install(FILES "${PROJECT_SOURCE_DIR}/include/tlvjson/tlvjson.h" DESTINATION include/tlvjson)
install(TARGETS tlvjsonc LIBRARY DESTINATION lib)
