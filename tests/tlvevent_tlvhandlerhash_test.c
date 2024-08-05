#include "tlv/tlvevent.h"
#include "tlv/tlvserver.h"
#include <stdio.h>

void tlvhandler1(TLV *tlv, void *arg) {
    printf("tlvhandler1\n");
}

void tlvhandler2(TLV *tlv, void *arg) {
    printf("tlvhandler2\n");
}

int main(int argc, char *argv[])
{
    TLVServer *tlvServer = malloc(sizeof(TLVServer));
    memset(tlvServer, 0, sizeof(TLVServer));

    tlvServer->handlerHashHead = NULL;
    tlvhandler_hash_add(&tlvServer->handlerHashHead, "test1", tlvhandler1);
    tlvhandler_hash_add(&tlvServer->handlerHashHead, "test2", tlvhandler2);

    TLVHandlerHash *hash = tlvhandler_hash_find(&tlvServer->handlerHashHead, "test2");

    printf("xxxx\n");
    if (hash) {
        printf("找到了\n");
        hash->handler(NULL, NULL);
    }

    tlvhandler_hash_free(&tlvServer->handlerHashHead);

    free(tlvServer);
    return 0;
}