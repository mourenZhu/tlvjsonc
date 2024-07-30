#include <tlvjson/tlvjson.h>
#include <stdlib.h>

TLVJSON* tlvjson_new()
{
    TLVJSON *tlvjson = malloc(sizeof(TLVJSON));
    tlvjson->type = NULL;
    tlvjson->length = NULL;
    tlvjson->value = NULL;
    return tlvjson;
}

void tlvjson_free(TLVJSON **tlvjson)
{
    if (tlvjson != NULL && *tlvjson != NULL) {
        SAFE_FREE((*tlvjson)->type);
        SAFE_FREE((*tlvjson)->length);
        SAFE_FREE((*tlvjson)->value);
        SAFE_FREE(*tlvjson);
    }
}

TLVJSON_CBArg* tlvjson_cbarg_new()
{
    TLVJSON_CBArg *tlvjsonCbArg = malloc(sizeof(TLVJSON_CBArg));
    tlvjsonCbArg->tlvjson = NULL;
    tlvjsonCbArg->value_total_len = 0;
    tlvjsonCbArg->value_current_len = 0;
    return tlvjsonCbArg;
}

void tlvjson_cbarg_free(TLVJSON_CBArg **tlvjsonCbArg)
{
    if (tlvjsonCbArg != NULL && *tlvjsonCbArg != NULL) {
        if ((*tlvjsonCbArg)->tlvjson != NULL) {
            tlvjson_free(&(*tlvjsonCbArg)->tlvjson);
        }
        SAFE_FREE(*tlvjsonCbArg);
    }
}
