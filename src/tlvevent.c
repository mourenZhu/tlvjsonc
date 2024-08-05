#include "tlv/tlvevent.h"
#include <string.h>
#include <stdlib.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "tlv/log.h"
#include "tlv/uthash.h"

TLV_CBArg* tlv_cbarg_new()
{
    TLV_CBArg *tlvCbArg = malloc(sizeof(TLV_CBArg));
    memset(tlvCbArg, 0, sizeof(TLV_CBArg));
    return tlvCbArg;
}

void tlv_cbarg_free(TLV_CBArg **tlvCbArg)
{
    if (tlvCbArg != NULL && *tlvCbArg != NULL) {
        if ((*tlvCbArg)->tlv != NULL) {
            tlv_free(&(*tlvCbArg)->tlv);
        }
        SAFE_FREE(*tlvCbArg);
    }
}

void tlv_cbarg_reset(TLV_CBArg *tlvCbArg)
{
    tlv_reset(tlvCbArg->tlv);
    memset(tlvCbArg->len_char, 0, sizeof(size_t));
    tlvCbArg->len_current_len = 0;
    tlvCbArg->value_current_len = 0;
}

int tlv_read_with_bufferevent(struct bufferevent *bev, TLV_CBArg *tlvCbArg)
{
    struct evbuffer *input = bufferevent_get_input(bev);
    int fd = bufferevent_getfd(bev);
    size_t in_buffer_len = 0;

    log_debug("fd: %d, in_buffer_len: %ld", fd, in_buffer_len);
    TLV *tlv = tlvCbArg->tlv;
    if (tlv->type == NULL) {
        tlv->type = evbuffer_readln(input, NULL, EVBUFFER_EOL_CRLF);
        log_debug("fd: %d, type: %s", fd, tlv->type);
        if (tlv->type == NULL) {
            return 0;
        }
    }
    if (tlvCbArg->len_current_len < sizeof(size_t)) {
        tlvCbArg->len_current_len += bufferevent_read(
                bev, tlvCbArg->len_char + tlvCbArg->len_current_len, sizeof(size_t) - tlvCbArg->len_current_len);
        log_debug("fd: %d, len_current_len: %ld", fd, tlvCbArg->len_current_len);
        if (tlvCbArg->len_current_len < 8) {
            return 0;
        }
        memcpy(&tlv->length, tlvCbArg->len_char, sizeof(size_t));
        log_debug("fd: %d, tlv->length: %ld", fd, tlv->length);
    }

    if (tlv->value == NULL) {
        size_t value_size = sizeof(char) * (tlv->length + 1);
        tlv->value = malloc(value_size);
        memset(tlv->value, 0, value_size);
    }

    tlvCbArg->value_current_len += bufferevent_read(
            bev, tlv->value + tlvCbArg->value_current_len,tlv->length - tlvCbArg->value_current_len);
    log_debug("fd: %d, value_current_len: %ld", fd, tlvCbArg->value_current_len);
    if (tlvCbArg->value_current_len < tlv->length) {
        return 0;
    }
    return 1;
}


int tlv_send(struct bufferevent *bev, TLV *tlv)
{
    size_t type_len = strlen(tlv->type) + 2;
    size_t len_len = sizeof(tlv->length);
    size_t val_len = tlv->length;
    size_t total_len = type_len + len_len + val_len;
    char *tlvdata = malloc(total_len);
    char type_char[type_len];
    sprintf(type_char, "%s\r\n", tlv->type); // 添加\r\n
    memcpy(tlvdata, type_char, type_len); // 添加完整type
    memcpy(tlvdata+type_len, &tlv->length, sizeof(tlv->length)); // 添加length
    memcpy(tlvdata+type_len+len_len, tlv->value, tlv->length);
    int ret;
    ret = bufferevent_write(bev, tlvdata, total_len);
    free(tlvdata);
    return ret;
}


int tlvhandler_hash_add(TLVHandlerHash **head, const char *type, tlvhandler handler)
{
    TLVHandlerHash *handlerHash = malloc(sizeof(TLVHandlerHash));
    handlerHash->type = malloc(strlen(type) + 1);
    strcpy(handlerHash->type, type);
    handlerHash->handler = handler;
    HASH_ADD_KEYPTR(hh, *head, handlerHash->type, strlen(type), handlerHash);
    return 0;
}

TLVHandlerHash *tlvhandler_hash_find(TLVHandlerHash **head, const char *type)
{
    TLVHandlerHash *hash = NULL;
    HASH_FIND_STR(*head, type, hash);
    return hash;
}

void tlvhandler_hash_free(TLVHandlerHash **head)
{
    TLVHandlerHash *handlerHash, *handlerHashTmp;
    /* free the hash table contents */
    HASH_ITER(hh, *head, handlerHash, handlerHashTmp) {
        HASH_DEL(*head, handlerHash);
        SAFE_FREE(handlerHash->type);
        SAFE_FREE(handlerHash);
    }
}
