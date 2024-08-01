#ifndef TLVCLIENT_H
#define TLVCLIENT_H
#include <stdlib.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#define IPV4_ADDR_LEN 16
#define IPV6_ADDR_LEN 40
#define CLIENTID_LENGTH 128

typedef struct tlvclient_conf
{
    char server_ipv4_addr[IPV4_ADDR_LEN];
    char server_ipv6_addr[IPV6_ADDR_LEN];
    u_int16_t server_port;
    char clientid[CLIENTID_LENGTH+1];
} TLVClientConf;


/**
 * 初始化客户端配置结构体
 * @param cid client id
 * @param s_ipv4 server ipv4 addr
 * @param s_ipv6 server ipv6 addr
 * @param s_port server port
 * @return 0 正常, -1 异常
 */
int tlvcconf_init(TLVClientConf *tlvClientConf, const char *cid, const char *s_ipv4, const char *s_ipv6, u_int16_t s_port);

/**
 *
 * @param tlvClientConf
 * @return 0 正常，-1异常
 */
int tlvclient_start_by_conf(TLVClientConf *tlvClientConf);

/**
 *
 * @param tlvClientConf
 * @return 0正常，-1异常
 */
int tlvclient_exit(TLVClientConf *tlvClientConf);


typedef struct tlvclient
{
    struct event_base *base;
    struct bufferevent *bufferevent;
} TLVJClient;

/**
 * 创建一个结构体，并返回指针
 * @return
 */
TLVJClient *tlvclient_new();


/**
 * 销毁
 * @param pTlvjClient
 */
void tlvclient_free(TLVJClient **pTlvjClient);


int tlvclient_send_tlvjson();

#endif