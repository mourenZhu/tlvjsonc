#ifndef TLVJCLIENT_H
#define TLVJCLIENT_H
#include <stdlib.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#define IPV4_ADDR_LEN 16
#define IPV6_ADDR_LEN 40
#define CLIENTID_LENGTH 128

typedef struct tlvjclient_conf
{
    char server_ipv4_addr[IPV4_ADDR_LEN];
    char server_ipv6_addr[IPV6_ADDR_LEN];
    u_int16_t server_port;
    char clientid[CLIENTID_LENGTH+1];
} TLVJClientConf;


/**
 * 初始化客户端配置结构体
 * @param cid client id
 * @param s_ipv4 server ipv4 addr
 * @param s_ipv6 server ipv6 addr
 * @param s_port server port
 * @return 0 正常, -1 异常
 */
int tlvjcconf_init(TLVJClientConf *tlvjClientConf, const char *cid, const char *s_ipv4, const char *s_ipv6, u_int16_t s_port);

/**
 *
 * @param tlvjClientConf
 * @return 0 正常，-1异常
 */
int tlvjclient_start_by_conf(TLVJClientConf *tlvjClientConf);

/**
 *
 * @param tlvjClientConf
 * @return 0正常，-1异常
 */
int tlvjclient_exit(TLVJClientConf *tlvjClientConf);


typedef struct tlvjclient
{
    struct event_base *base;
    struct bufferevent *bufferevent;
} TLVJClient;

/**
 * 创建一个结构体，并返回指针
 * @return
 */
TLVJClient *tlvjclient_new();


/**
 * 销毁
 * @param pTlvjClient
 */
void tlvjclient_free(TLVJClient **pTlvjClient);


int tlvjclient_send_tlvjson();

#endif