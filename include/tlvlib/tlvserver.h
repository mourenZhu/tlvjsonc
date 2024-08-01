#ifndef TLVSERVER_H
#define TLVSERVER_H
#include <stdlib.h>
#include <event2/listener.h>

#define IPV4_ADDR_LEN 16
#define IPV6_ADDR_LEN 40


typedef struct tlvserver_conf
{
    char ipv4_addr[IPV4_ADDR_LEN];
    char ipv6_addr[IPV6_ADDR_LEN];
    u_int16_t server_port;
    struct evconnlistener *_listener;
} TLVServerConf;

/**
 * new
 * @return
 */
TLVServerConf *tlvsconf_new();

/**
 * free
 * @param tlvServConf
 */
void tlvsconf_free(TLVServerConf **pTlvServerConf);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, -1 if an error occurred, or 1 if we exited because no events were pending or active.
 */
int tlvserver_start_by_conf(TLVServerConf *tlvServerConf);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, or -1 if an error occurred
 */
int tlvserver_exit(TLVServerConf *tlvServerConf);


#endif