#ifndef TLVJSERVER_H
#define TLVJSERVER_H
#include <stdlib.h>
#include <event2/listener.h>

#define IPV4_ADDR_LEN 16
#define IPV6_ADDR_LEN 40


typedef struct tlvserv_conf
{
    char ipv4_addr[IPV4_ADDR_LEN];
    char ipv6_addr[IPV6_ADDR_LEN];
    u_int16_t server_port;
    struct evconnlistener *listener;
} TLVServConf;

/**
 * init
 * @param tlvServConf
 * @return
 */
int tlvjserver_init(TLVServConf *tlvServConf);

/**
 * free
 * @param tlvServConf
 */
void tlvjserver_free(TLVServConf *tlvServConf);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, -1 if an error occurred, or 1 if we exited because no events were pending or active.
 */
int tlvjserver_start_by_conf(TLVServConf *tlvServConf);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, or -1 if an error occurred
 */
int tlvjserver_exit(TLVServConf *tlvServConf);


#endif