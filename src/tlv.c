#include "tlv/tlv.h"
#include <stdlib.h>
#include <string.h>

TLV* tlv_new()
{
    TLV *tlv = malloc(sizeof(TLV));
    tlv->type = NULL;
    tlv->length = 0;
    tlv->value = NULL;
    return tlv;
}

void tlv_free(TLV **tlv)
{
    if (tlv != NULL && *tlv != NULL) {
        SAFE_FREE((*tlv)->type);
        SAFE_FREE((*tlv)->value);
        SAFE_FREE(*tlv);
    }
}

void tlv_reset(TLV *tlv)
{
    if (tlv != NULL) {
        SAFE_FREE(tlv->type);
        SAFE_FREE(tlv->value);
    }
}

TLV_CBArg* tlv_cbarg_new()
{
    TLV_CBArg *tlvCbArg = malloc(sizeof(TLV_CBArg));
    memset(tlvCbArg, 0, sizeof(TLV_CBArg));
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
    memset(tlvCbArg->len_char, 0, sizeof(size_t));
    tlvCbArg->len_current_len = 0;
    tlvCbArg->value_current_len = 0;
}
