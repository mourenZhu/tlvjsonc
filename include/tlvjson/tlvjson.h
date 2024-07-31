#ifndef TLVJSON_H
#define TLVJSON_H

#include <event2/bufferevent.h>

#define SAFE_FREE(ptr) do { \
    if (ptr) {              \
        free(ptr);          \
        ptr = NULL;         \
    }                       \
} while (0)

typedef struct tlv {
    char *type;
    char *length;
    char *value;
} TLV;


/**
 * init tlv
 *
 * @return NULL if an error occurred
 */
TLV* tlv_new();

/**
 * free tlv
 * @param tlv
 */
void tlv_free(TLV **tlv);

/**
 * 重置tlv结构体内的数据
 * @param tlv
 */
void tlv_reset(TLV *tlv);

int tlv_read4bufferevent(TLV *tlv, struct bufferevent *bev);


typedef struct tlv_cbarg{
    TLV *tlv;
    size_t value_total_len;
    size_t value_current_len;
} TLV_CBArg;

/**
 * init
 *
 * @return NULL if an error occurred
 */
TLV_CBArg* tlv_cbarg_new();

/**
 * free
 * @param tlvCbArg
 */
void tlv_cbarg_free(TLV_CBArg **tlvCbArg);

/**
 * 重置tlvCbArg结构体内的值
 * @param tlvCbArg
 */
void tlv_cbarg_reset(TLV_CBArg *tlvCbArg);
#endif