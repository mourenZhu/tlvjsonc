cmake_minimum_required(VERSION 3.22)
project(dcloudc VERSION 0.0.1
        DESCRIPTION "tlvlib example simple server client"
        LANGUAGES C)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_library(LIBEVENT2_CORE event_core)
find_library(cJSON cjson)
find_library(tlv_core tlv_core)
find_library(tlvserver tlvserver)
find_library(tlvclient tlvclient)

add_executable(client client.c)
add_executable(server server.c)

target_link_libraries(client PRIVATE ${tlvclient} ${tlv_core} ${LIBEVENT2_CORE} ${cJSON})
target_link_libraries(server PRIVATE ${tlvserver} ${tlv_core} ${LIBEVENT2_CORE} ${cJSON})
