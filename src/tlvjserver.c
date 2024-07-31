#include <tlvjson/tlvjserver.h>
#include <string.h>
#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <stdio.h>
#include "tlvjson/log.h"
#include "tlvjson/tlvjson.h"

TLVJServerConf *tlvjsconf_new()
{
    TLVJServerConf *tlvjServerConf = malloc(sizeof(TLVJServerConf));
    memset(tlvjServerConf->ipv4_addr, 0, sizeof(tlvjServerConf->ipv4_addr));
    memset(tlvjServerConf->ipv6_addr, 0, sizeof(tlvjServerConf->ipv6_addr));
    tlvjServerConf->server_port = 0;
    tlvjServerConf->_listener = NULL;
    return tlvjServerConf;
}

void tlvjsconf_free(TLVJServerConf **pTlvjServerConf)
{
    SAFE_FREE(*pTlvjServerConf);
}

static void
server_read_cb(struct bufferevent *bev, void *ctx)
{
    /* This callback is invoked when there is data to read on bev. */
    struct evbuffer *input = bufferevent_get_input(bev);
    TLV_CBArg *tlvCbArg = (TLV_CBArg *)ctx;
    TLV *tlv = tlvCbArg->tlv;
    if (tlv->type == NULL) {
        tlv->type = evbuffer_readln(input, NULL, EVBUFFER_EOL_CRLF);
        if (tlv->type == NULL) {
            return;
        }
    }
    if (tlv->length == NULL) {
        tlv->length = evbuffer_readln(input, NULL, EVBUFFER_EOL_CRLF);
        if (tlv->length == NULL) {
            return;
        }

    }
    if (tlv->value == NULL) {
        char *endptr;
        size_t length = strtol(tlv->length, &endptr, 10);
        if (*endptr != '\0') {
            log_error("recv err length value, errmsg: %s", endptr);
            bufferevent_free(bev);
            return;
        }
        tlvCbArg->value_total_len = length;
        size_t value_size = sizeof(char ) * (length + 1);
        tlv->value = malloc(value_size);
        memset(tlv->value, 0, value_size);
    }
    size_t read_len = 0;
    read_len = bufferevent_read(bev, tlv->value + tlvCbArg->value_current_len,
                                tlvCbArg->value_total_len - tlvCbArg->value_current_len);
    tlvCbArg->value_current_len += read_len;
    if (tlvCbArg->value_current_len < tlvCbArg->value_total_len) {
        return;
    }
    log_info("tlv: type: %s, length: %s, value: %s", tlv->type, tlv->length, tlv->value);
    tlv_cbarg_reset(tlvCbArg);
}

static void
server_event_cb(struct bufferevent *bev, short events, void *ctx)
{
    TLV_CBArg *tlvCbArg = (TLV_CBArg*) ctx;
    if (events & BEV_EVENT_ERROR)
        log_error("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        log_info("events: %d ,event close", events);
        tlv_cbarg_free(&tlvCbArg);
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

    TLV *tlv = tlv_new();
    TLV_CBArg *tlvCbArg = tlv_cbarg_new();
    tlvCbArg->tlv = tlv;
    bufferevent_setcb(bev, server_read_cb, NULL, server_event_cb, tlvCbArg);

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

int tlvjserver_start(TLVJServerConf *tlvjServerConf, struct sockaddr_in sin, unsigned flags, int backlog)
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

    tlvjServerConf->_listener = listener;

    evconnlistener_set_error_cb(listener, accept_error_cb);

    int ret;
    ret = event_base_dispatch(base);
    return ret;

listener_err:
    evconnlistener_free(listener);
    return -1;
}

int tlvjserver_start_by_conf(TLVJServerConf *tlvjServerConf)
{
    struct sockaddr_in sin;

    int port = tlvjServerConf->server_port;

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
    sin.sin_addr.s_addr = inet_addr(tlvjServerConf->ipv4_addr);
    /* Listen on the given port. */
    sin.sin_port = htons(port);

    return tlvjserver_start(tlvjServerConf, sin, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1);
}
