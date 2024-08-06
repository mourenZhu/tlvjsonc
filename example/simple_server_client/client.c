#include <stdio.h>
#include <tlv/tlvclient.h>
#include <tlv/tlvevent.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *client_thread(void *arg);

void client_tlv_handler(TLV *tlv, struct bufferevent *bev, void *ctx)
{
    printf("client tlv type: %s, length: %ld, value: %s\n", tlv->type, tlv->length, (char *)tlv->value);
}

int main(int argc, char *argv[])
{
    printf("client\n");
    TLVClientConf clientConf;
    tlvcconf_init(&clientConf, "123456", "127.0.0.1", "", 10816);
    TLVClient *tlvClient = tlvclient_new_with_conf(&clientConf);
    tlvClient->tlvHandler = client_tlv_handler;

    pthread_t client_thread_t;
    pthread_create(&client_thread_t, NULL, client_thread, (void *)tlvClient);

    sleep(2);
    char msg[128];
    int ret = 0;
    TLV *tlv = NULL;
    for (int i = 0; i < 10; i++) {
        sprintf(msg, "hello world %d !", i);
        tlv = tlv_new_copy("hello", msg, strlen(msg) + 1);
        ret = tlv_send(tlvClient->bev, tlv);
        printf("ret: %d\n", ret);
        tlv_free(&tlv);
    }
    pthread_join(client_thread_t, NULL);
    tlvclient_free(&tlvClient);
    printf("end\n");
    return 0;
}

void *client_thread(void *arg)
{
    TLVClient *tlvClient = (TLVClient *)arg;
    tlvclient_start(tlvClient);
    printf("client_thread end\n");
    return NULL;
}