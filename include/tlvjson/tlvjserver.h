#ifndef TLVJSERVER_H
#define TLVJSERVER_H
#include <stdlib.h>
#include <event2/listener.h>

#define IPV4_ADDR_LEN 16
#define IPV6_ADDR_LEN 40


typedef struct tlvjserver_conf
{
    char ipv4_addr[IPV4_ADDR_LEN];
    char ipv6_addr[IPV6_ADDR_LEN];
    u_int16_t server_port;
    struct evconnlistener *_listener;
} TLVJServerConf;

/**
 * new
 * @return
 */
TLVJServerConf *tlvjsconf_new();

/**
 * free
 * @param tlvServConf
 */
void tlvjsconf_free(TLVJServerConf **pTlvjServerConf);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, -1 if an error occurred, or 1 if we exited because no events were pending or active.
 */
int tlvjserver_start_by_conf(TLVJServerConf *tlvjServerConf);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, or -1 if an error occurred
 */
int tlvjserver_exit(TLVJServerConf *tlvjServerConf);


#endif