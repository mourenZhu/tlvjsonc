#include <tlvlib/tlvclient.h>
#include "tlvlib/tlv.h"
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include "tlvlib/log.h"

int tlvcconf_init(TLVClientConf *tlvClientConf, const char *cid, const char *s_ipv4, const char *s_ipv6, u_int16_t s_port)
{
    memset(tlvClientConf, 0, sizeof(TLVClientConf));
    strcpy(tlvClientConf->clientid, cid);
    strcpy(tlvClientConf->server_ipv4_addr, s_ipv4);
    strcpy(tlvClientConf->server_ipv6_addr, s_ipv6);
    tlvClientConf->server_port = s_port;
    return 0;
}

static void
event_cb(struct bufferevent *bev, short events, void *ctx) {
    if (events & BEV_EVENT_CONNECTED) {
        printf("Connected to server\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Error in connection\n");
        bufferevent_free(bev);
    } else if (events & BEV_EVENT_EOF) {
        printf("Connection closed\n");
        bufferevent_free(bev);
    }
}

static void
read_cb(struct bufferevent *bev, void *ctx) {
    char buffer[256];
    size_t n;

    while ((n = bufferevent_read(bev, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, n, stdout);
    }
}


int tlvclient_start_by_conf(TLVClientConf *tlvClientConf)
{
    struct event_base *base = event_base_new();
    if (!base) {
        log_error("Could not initialize libevent!");
        return -1;
    }

    struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        log_error("Error constructing bufferevent!");
        event_base_free(base);
        return 1;
    }

    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);

    if (bufferevent_socket_connect_hostname(
            bev, NULL, AF_INET, tlvClientConf->server_ipv4_addr, tlvClientConf->server_port) < 0) {
        log_error("Error starting connection");
        bufferevent_free(bev);
        event_base_free(base);
        return -1;
    }

    bufferevent_enable(bev, EV_READ | EV_WRITE);
    event_base_dispatch(base);
    event_base_free(base);
    return 0;
}
