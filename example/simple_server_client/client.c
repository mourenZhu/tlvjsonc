#include <stdio.h>
#include <tlv/tlvclient.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *client_thread(void *arg);

int main(int argc, char *argv[])
{
    printf("client\n");
    TLVClientConf clientConf;
    tlvcconf_init(&clientConf, "123456", "127.0.0.1", "", 8964);
    TLVClient *tlvClient = tlvclient_new_with_conf(&clientConf);
    pthread_t client_thread_t;
    pthread_create(&client_thread_t, NULL, client_thread, (void *)tlvClient);
    sleep(2);
    char msg[10];
    int ret = 0;
    for (int i = 0; i < 10; i++) {
        sprintf(msg, "test: %d", i);
//        ret = tlvclient_send_string(tlvClient, "test", strlen(msg), msg);
        printf("ret: %d\n", ret);
    }
//    pthread_join(client_thread_t, NULL);
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