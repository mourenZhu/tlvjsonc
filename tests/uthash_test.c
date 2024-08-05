#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "tlv/uthash.h"

//struct my_struct {
//    const char *name;          /* key */
//    int id;
//    UT_hash_handle hh;         /* makes this structure hashable */
//};

typedef void (*tlvhandler) (void *tlv, void *arg);

typedef struct tlvhandler_hash
{
    char *type;
    tlvhandler handler;
    UT_hash_handle hh;
}TLVHandlerHash;


int main(int argc, char *argv[]) {
//    const char *names[] = { "joe", "bob", "betty", NULL };
//    struct my_struct *s, *tmp, *users = NULL;
//
//    for (int i = 0; names[i]; ++i) {
//        s = (struct my_struct *)malloc(sizeof *s);
//        s->name = names[i];
//        s->id = i;
//        HASH_ADD_KEYPTR(hh, users, s->name, strlen(s->name), s);
//    }
//
//    HASH_FIND_STR(users, "betty", s);
//    if (s) printf("betty's id is %d\n", s->id);
//
//    /* free the hash table contents */
//    HASH_ITER(hh, users, s, tmp) {
//        HASH_DEL(users, s);
//        free(s);
//    }
    TLVHandlerHash *head = NULL;
    TLVHandlerHash *tmp = NULL;
    TLVHandlerHash *s, *handlerHash1 = malloc(sizeof(TLVHandlerHash));
    handlerHash1->type = malloc(sizeof(char ) * 10);
    sprintf(handlerHash1->type, "test1");
    HASH_ADD_KEYPTR(hh, head, handlerHash1->type, strlen(handlerHash1->type), handlerHash1);
    if (head) {
        printf("head not null\n");
        printf("type: %s\n", head->type);
    }
    HASH_FIND_STR(head, "test1", tmp);
    if (tmp) {
        printf("type: %s\n", tmp->type);
    }

    HASH_ITER(hh, head, s, tmp) {
        HASH_DEL(head, s);
    }
    free(handlerHash1);
    return 0;
}