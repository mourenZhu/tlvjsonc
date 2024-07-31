#include <stdio.h>
#include <string.h>
#include <tlvjson/tlvjserver.h>

int main(int argc, char *argv[])
{
    printf("server start\n");
    TLVJServerConf *tlvjserverConf = tlvjsconf_new();
    tlvjserverConf->server_port = 8964;
    strcpy(tlvjserverConf->ipv4_addr, "0.0.0.0");
    tlvjserver_start_by_conf(tlvjserverConf);
    tlvjsconf_free(&tlvjserverConf);
    return 0;
}