#include "tlv/tlvserver.h"
#include <string.h>
#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <stdio.h>
#include "tlv//log.h"
#include "tlv/tlv.h"
#include "tlv/tlvevent.h"

int tlvsconf_init(TLVServerConf *tlvServerConf, const char *ipv4, const char *ipv6, u_int16_t port)
{
    strcpy(tlvServerConf->ipv4_addr, ipv4);
    strcpy(tlvServerConf->ipv6_addr, ipv6);
    tlvServerConf->server_port = port;
    return 0;
}

static void
server_read_cb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input = bufferevent_get_input(bev);
    log_debug("read cd buffer length: %ld", evbuffer_get_length(input));
    TLV *tlv = NULL;
    TLVServer *tlvServer = (TLVServer *)ctx;
    while ((tlv = tlv_read_new_with_bufferevent(bev))) {
        if (tlvServer->tlvHandler) {
            tlvServer->tlvHandler(tlv, bev, NULL);
        }
        tlv_free(&tlv);
    }
}

static void
server_event_cb(struct bufferevent *bev, short events, void *ctx)
{
    TLVServer *tlvServer = (TLVServer *)ctx;
    if (tlvServer->eventHandler) {
        tlvServer->eventHandler(bev, events, NULL);
    }
    if (events & BEV_EVENT_ERROR)
        log_error("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        log_debug("events: %d ,event close", events);
        bufferevent_free(bev);
    }
}

static void
accept_conn_cb(struct evconnlistener *listener,
               evutil_socket_t fd, struct sockaddr *address, int socklen,
               void *ctx)
{
    log_info("new conn, fd: %d", fd);
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(
            base, fd, BEV_OPT_CLOSE_ON_FREE);
    struct timeval timeout_read = {.tv_sec = 5};
    TLVServer *tlvServer = (TLVServer *)ctx;

    bufferevent_set_timeouts(bev, &timeout_read, NULL);

    bufferevent_setcb(bev, server_read_cb, NULL, server_event_cb, tlvServer);

    bufferevent_enable(bev, EV_READ|EV_WRITE);

    if (tlvServer->acceptConnHandler) {
        tlvServer->acceptConnHandler(bev, address, socklen, NULL);
    }
}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    log_error("Got an error %d (%s) on the listener. "
              "Shutting down.\n", err, evutil_socket_error_to_string(err));
    TLVServer *tlvServer = (TLVServer *)ctx;
    if (tlvServer->acceptErrorHandler) {
        tlvServer->acceptErrorHandler(NULL);
    }
    event_base_loopexit(base, NULL);
}

TLVServer* tlvserver_new_with_conf(TLVServerConf *tlvServerConf)
{
    TLVServer *tlvServer = malloc(sizeof(TLVServer));
    memset(tlvServer, 0, sizeof(TLVServer));
    tlvServer->conf = tlvServerConf;

    struct sockaddr_in sin;

    int port = tlvServerConf->server_port;

    if (port<=0 || port>65535) {
        perror("Invalid server port");
        return NULL;
    }

    /* Clear the sockaddr before using it, in case there are extra
     * platform-specific fields that can mess us up. */
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = inet_addr(tlvServerConf->ipv4_addr);
    /* Listen on the given port. */
    sin.sin_port = htons(port);

    struct event_base *base;
    struct evconnlistener *listener;

    base = event_base_new();
    if (!base) {
        log_error("Couldn't open event base");
        return NULL;
    }

    listener = evconnlistener_new_bind(base, accept_conn_cb, tlvServer,
                                       LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
                                       (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        perror("Couldn't create listener");
        return NULL;
    }

    int fd = evconnlistener_get_fd(listener);
    if (evutil_make_socket_nonblocking(fd) != 0) {
        perror("couldn't make socket nonblocking");
        evconnlistener_free(listener);
        return NULL;
    }
    tlvServer->listener = listener;
    evconnlistener_set_error_cb(listener, accept_error_cb);

    return tlvServer;
}


int tlvserver_start(TLVServer *tlvServer)
{
    struct event_base *base = evconnlistener_get_base(tlvServer->listener);
    return event_base_dispatch(base);
}
