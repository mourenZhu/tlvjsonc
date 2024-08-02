#include <stdio.h>
#include <string.h>
#include <tlv//tlvserver.h>

int main(int argc, char *argv[])
{
    printf("server start\n");
    TLVServerConf *tlvServerConf = tlvsconf_new();
    tlvServerConf->server_port = 8964;
    strcpy(tlvServerConf->ipv4_addr, "0.0.0.0");
    tlvserver_start_by_conf(tlvServerConf);
    tlvsconf_free(&tlvServerConf);
    return 0;
}