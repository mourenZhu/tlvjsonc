#ifndef TLVEVENT_H
#define TLVEVENT_H

#include "tlv.h"
#include "uthash.h"

typedef struct tlv_cbarg{
    TLV *tlv;
    char len_char[sizeof(size_t)]; // 先用字符串接收size_t
    size_t len_current_len;
    size_t value_current_len;
} TLV_CBArg;


typedef void (*tlvhandler) (TLV *tlv, void *arg);

typedef struct tlvhandler_hash
{
    char *type;
    tlvhandler handler;
    UT_hash_handle hh;
}TLVHandlerHash;

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

/**
 * 通过bufferevent读取tlv
 * @param bev bufferevent
 * @param tlvCbArg
 * @return 0还未全读取，-1异常，1完成读取
 */
int tlv_read_with_bufferevent(struct bufferevent *bev, TLV_CBArg *tlvCbArg);

/**
 * tlv 通过bufferevent发送tlv数据
 *
 * @param bev
 * @param tlv
 * @return 0 if successful, or -1 if an error occurred
 */
int tlv_send(struct bufferevent *bev, TLV *tlv);


int tlvhandler_hash_add(TLVHandlerHash **head, const char *type, tlvhandler handler);

TLVHandlerHash *tlvhandler_hash_find(TLVHandlerHash **head, const char *type);

void tlvhandler_hash_free(TLVHandlerHash **head);

#endif