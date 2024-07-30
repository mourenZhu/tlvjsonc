#include <tlvjson/tlvjserver.h>
#include <string.h>
#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <stdio.h>
#include "tlvjson/log.h"
#include "tlvjson/tlvjson.h"

int tlvjserver_init(TLVServConf *tlvServConf)
{
    memset(tlvServConf->ipv4_addr, 0, sizeof(tlvServConf->ipv4_addr));
    memset(tlvServConf->ipv6_addr, 0, sizeof(tlvServConf->ipv6_addr));
    tlvServConf->server_port = 0;
    tlvServConf->listener = NULL;
    return 0;
}

void tlvjserver_free(TLVServConf *tlvServConf)
{

}

static void
server_read_cb(struct bufferevent *bev, void *ctx)
{
    /* This callback is invoked when there is data to read on bev. */
    struct evbuffer *input = bufferevent_get_input(bev);
    TLVJSON_CBArg *tlvjsonCbArg = (TLVJSON_CBArg *)ctx;
    TLVJSON *tlvjson = tlvjsonCbArg->tlvjson;
    if (tlvjson->type == NULL) {
        tlvjson->type = evbuffer_readln(input, NULL, EVBUFFER_EOL_CRLF);
        if (tlvjson->type == NULL) {
            return;
        }
    }
    if (tlvjson->length == NULL) {
        tlvjson->length = evbuffer_readln(input, NULL, EVBUFFER_EOL_CRLF);
        if (tlvjson->length == NULL) {
            return;
        }

    }
    if (tlvjson->value == NULL) {
        char *endptr;
        size_t length = strtol(tlvjson->length, &endptr, 10);
        if (*endptr != '\0') {
            log_error("recv err length value, errmsg: %s", endptr);
            bufferevent_free(bev);
            return;
        }
        tlvjsonCbArg->value_total_len = length;
        size_t value_size = sizeof(char ) * (length + 1);
        tlvjson->value = malloc(value_size);
        memset(tlvjson->value, 0, value_size);
    }


}

static void
server_event_cb(struct bufferevent *bev, short events, void *ctx)
{
    TLVJSON_CBArg *tlvjsonCbArg = (TLVJSON_CBArg*) ctx;
    if (events & BEV_EVENT_ERROR)
        log_error("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        log_info("events: %d ,event close", events);
        tlvjson_cbarg_free(&tlvjsonCbArg);
        bufferevent_free(bev);
    }
}

static void
accept_conn_cb(struct evconnlistener *listener,
               evutil_socket_t fd, struct sockaddr *address, int socklen,
               void *ctx)
{
    /* We got a new connection! Set up a bufferevent for it. */
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(
            base, fd, BEV_OPT_CLOSE_ON_FREE);
    struct timeval timeout_read = {.tv_sec = 5};

    bufferevent_set_timeouts(bev, &timeout_read, NULL);

    TLVJSON *tlvjson = tlvjson_new();
    TLVJSON_CBArg *tlvjsonCbArg = tlvjson_cbarg_new();
    tlvjsonCbArg->tlvjson = tlvjson;
    bufferevent_setcb(bev, server_read_cb, NULL, server_event_cb, tlvjsonCbArg);

    bufferevent_enable(bev, EV_READ|EV_WRITE);
}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    printf("Got an error %d (%s) on the listener. "
              "Shutting down.\n", err, evutil_socket_error_to_string(err));
    event_base_loopexit(base, NULL);
}

int tlvjserver_start(TLVServConf *tlvServConf, struct sockaddr_in sin, unsigned flags, int backlog)
{
    struct event_base *base;
    struct evconnlistener *listener;

    base = event_base_new();
    if (!base) {
        log_error("Couldn't open event base");
        return -1;
    }

    listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
                                       flags, backlog,
                                       (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        perror("Couldn't create listener");
        return -1;
    }

    int fd = evconnlistener_get_fd(listener);
    if (evutil_make_socket_nonblocking(fd) != 0) {
        perror("couldn't make socket nonblocking");
        goto listener_err;
    }

    tlvServConf->listener = listener;

    evconnlistener_set_error_cb(listener, accept_error_cb);

    int ret;
    ret = event_base_dispatch(base);
    return ret;

listener_err:
    evconnlistener_free(listener);
    return -1;
}

int tlvjserver_start_by_conf(TLVServConf *tlvServConf)
{
    struct sockaddr_in sin;

    int port = tlvServConf->server_port;

    if (port<=0 || port>65535) {
        perror("Invalid server port");
        return -1;
    }

    /* Clear the sockaddr before using it, in case there are extra
     * platform-specific fields that can mess us up. */
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = inet_addr(tlvServConf->ipv4_addr);
    /* Listen on the given port. */
    sin.sin_port = htons(port);

    return tlvjserver_start(tlvServConf, sin, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1);
}
