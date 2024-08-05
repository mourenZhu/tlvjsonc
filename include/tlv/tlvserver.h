#ifndef TLVSERVER_H
#define TLVSERVER_H
#include <stdlib.h>
#include <event2/listener.h>
#include "tlvevent.h"

#define IPV4_ADDR_LEN 16
#define IPV6_ADDR_LEN 40


typedef struct tlvserver_conf
{
    char ipv4_addr[IPV4_ADDR_LEN];
    char ipv6_addr[IPV6_ADDR_LEN];
    u_int16_t server_port;

} TLVServerConf;

typedef struct tlvserver
{
    TLVServerConf *conf;
    struct evconnlistener *listener;
    TLVHandlerHash *handlerHashHead;
}TLVServer;

/**
 * init
 * @return
 */
int tlvsconf_init(TLVServerConf *tlvServerConf, const char *ipv4, const char *ipv6, u_int16_t port);

/**
 *
 * @param tlvServerConf
 * @return
 */
TLVServer* tlvserver_new_with_conf(TLVServerConf *tlvServerConf);

/**
 *
 * @param tlvServer
 * @return 0 if successful, -1 if an error occurred, or 1 if we exited because no events were pending or active.
 */
int tlvserver_start(TLVServer *tlvServer);

/**
 *
 * @param tlvServConf
 * @return 0 if successful, or -1 if an error occurred
 */
int tlvserver_exit(TLVServer *tlvServer);


#endif