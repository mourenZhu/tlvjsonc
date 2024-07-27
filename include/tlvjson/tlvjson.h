#ifndef TLVJSON_H
#define TLVJSON_H

#include <event2/bufferevent.h>


typedef struct tlvjson {
    char *type;
    char *length;
    char *value;
} TLVJSON;


/**
 * init tlvjson
 *
 * @param tlvjson
 * @return 0 if successful, or -1 if an error occurred
 */
int tlvjson_init(TLVJSON *tlvjson);

/**
 * free tlvjson
 * @param tlvjson
 */
void tlvjson_free(TLVJSON *tlvjson);

int tlvjson_read4bufferevent(TLVJSON *tlvjson, struct bufferevent *bev);


#endif