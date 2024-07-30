# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

include_directories(${PROJECT_SOURCE_DIR}/include)

# Optionally glob, but only for CMake 3.12 or later:
file(GLOB TLVJSON_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/tlvjson/*.h")
# set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/device_cloud_client/conf.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(tlvjsonc STATIC tlvjson.c log.c ${TLVJSON_HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(tlvjsonc PUBLIC ${PROJECT_SOURCE_DIR}/include)

# IDEs should put the headers in a nice place
#source_group(
#        TREE "${PROJECT_SOURCE_DIR}/include"
#        PREFIX "TLVJSON_HEADER_LIST Header Files"
#        FILES ${TLVJSON_HEADER_LIST})

add_library(tlvjserverc STATIC tlvjserver.c log.c ${TLVJSON_HEADER_LIST})
target_include_directories(tlvjserverc PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(tlvjclientc STATIC tlvjclient.c log.c ${TLVJSON_HEADER_LIST})
target_include_directories(tlvjclientc PUBLIC ${PROJECT_SOURCE_DIR}/include)


install(FILES "${PROJECT_SOURCE_DIR}/include/tlvjson/tlvjson.h" DESTINATION include/tlvjson)
install(TARGETS tlvjsonc LIBRARY DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/include/tlvjson/tlvjserver.h" DESTINATION include/tlvjson)
install(TARGETS tlvjserverc LIBRARY DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/include/tlvjson/tlvjclient.h" DESTINATION include/tlvjson)
install(TARGETS tlvjclientc LIBRARY DESTINATION lib)

