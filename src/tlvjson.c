#include <tlvjson/tlvjson.h>
#include <stdlib.h>

TLV* tlv_new()
{
    TLV *tlv = malloc(sizeof(TLV));
    tlv->type = NULL;
    tlv->length = NULL;
    tlv->value = NULL;
    return tlv;
}

void tlv_free(TLV **tlv)
{
    if (tlv != NULL && *tlv != NULL) {
        SAFE_FREE((*tlv)->type);
        SAFE_FREE((*tlv)->length);
        SAFE_FREE((*tlv)->value);
        SAFE_FREE(*tlv);
    }
}

void tlv_reset(TLV *tlv)
{
    if (tlv != NULL) {
        SAFE_FREE(tlv->type);
        SAFE_FREE(tlv->length);
        SAFE_FREE(tlv->value);
    }
}

TLV_CBArg* tlv_cbarg_new()
{
    TLV_CBArg *tlvCbArg = malloc(sizeof(TLV_CBArg));
    tlvCbArg->tlv = NULL;
    tlvCbArg->value_total_len = 0;
    tlvCbArg->value_current_len = 0;
    return tlvCbArg;
}

void tlv_cbarg_free(TLV_CBArg **tlvCbArg)
{
    if (tlvCbArg != NULL && *tlvCbArg != NULL) {
        if ((*tlvCbArg)->tlv != NULL) {
            tlv_free(&(*tlvCbArg)->tlv);
        }
        SAFE_FREE(*tlvCbArg);
    }
}

void tlv_cbarg_reset(TLV_CBArg *tlvCbArg)
{
    tlv_reset(tlvCbArg->tlv);
    tlvCbArg->value_total_len = 0;
    tlvCbArg->value_current_len = 0;
}
