#ifndef TLVJSON_H
#define TLVJSON_H

#include <event2/bufferevent.h>

#define SAFE_FREE(ptr) \
{ \
    if (ptr) \
    { \
        free(ptr); \
        ptr = NULL; \
    } \
}

typedef struct tlvjson {
    char *type;
    char *length;
    char *value;
} TLVJSON;


/**
 * init tlvjson
 *
 * @return NULL if an error occurred
 */
TLVJSON* tlvjson_new();

/**
 * free tlvjson
 * @param tlvjson
 */
void tlvjson_free(TLVJSON **tlvjson);

int tlvjson_read4bufferevent(TLVJSON *tlvjson, struct bufferevent *bev);


typedef struct tlvjson_cbarg{
    TLVJSON *tlvjson;
    size_t value_total_len;
    size_t value_current_len;
} TLVJSON_CBArg;

/**
 * init
 *
 * @return NULL if an error occurred
 */
TLVJSON_CBArg* tlvjson_cbarg_new();

/**
 * free
 * @param tlvjsonCbArg
 */
void tlvjson_cbarg_free(TLVJSON_CBArg **tlvjsonCbArg);


#endif