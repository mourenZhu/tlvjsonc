#ifndef TLVJCLIENT_H
#define TLVJCLIENT_H
#include <stdlib.h>

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
 * 创建一个客户端配置结构体
 * @param cid client id
 * @param s_ipv4 server ipv4 addr
 * @param s_ipv6 server ipv6 addr
 * @param s_port server port
 * @return
 */
TLVJClientConf* tlvjcconf_new(const char *cid, const char *s_ipv4, const char *s_ipv6, u_int16_t s_port);

/**
 * 销毁结构体
 * @param pTlvjClientConf
 */
void tlvjcconf_free(TLVJClientConf **pTlvjClientConf);

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



#endif