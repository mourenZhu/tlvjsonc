# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

include_directories(${PROJECT_SOURCE_DIR}/include)

# Optionally glob, but only for CMake 3.12 or later:
file(GLOB TLV_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/tlvlib/*.h")
# set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/device_cloud_client/conf.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(tlvlib STATIC tlv.c log.c ${TLV_HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(tlvlib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# IDEs should put the headers in a nice place
#source_group(
#        TREE "${PROJECT_SOURCE_DIR}/include"
#        PREFIX "TLVJSON_HEADER_LIST Header Files"
#        FILES ${TLVJSON_HEADER_LIST})

add_library(tlvserver STATIC tlvserver.c log.c ${TLVJSON_HEADER_LIST})
target_include_directories(tlvserver PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(tlvclient STATIC tlvclient.c log.c ${TLVJSON_HEADER_LIST})
target_include_directories(tlvclient PUBLIC ${PROJECT_SOURCE_DIR}/include)


install(FILES "${PROJECT_SOURCE_DIR}/include/tlvlib/tlv.h" DESTINATION include/tlvlib)
install(TARGETS tlvlib LIBRARY DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/include/tlvlib/tlvserver.h" DESTINATION include/tlvlib)
install(TARGETS tlvserver LIBRARY DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/include/tlvlib/tlvclient.h" DESTINATION include/tlvlib)
install(TARGETS tlvclient LIBRARY DESTINATION lib)

