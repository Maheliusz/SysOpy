#include <string.h>
#include <stdlib.h>
#include "listAddressBook.h"

struct ListData *createData(char *name, char *surname, char *birthdate, char *email, char *phone, char *addr) {
    struct ListData *res = calloc(1, sizeof(struct ListData));
    if (name == NULL) res->name = NULL;
    else {
        res->name = calloc(strlen(name) + 1, sizeof(char));
        strcpy(res->name, name);
    }
    if (surname == NULL) res->surname = NULL;
    else {
        res->surname = calloc(strlen(surname) + 1, sizeof(char));
        strcpy(res->surname, surname);
    }
    if (email == NULL) res->email = NULL;
    else {
        res->email = calloc(strlen(email) + 1, sizeof(char));
        strcpy(res->email, email);
    }
    if (addr == NULL) res->addr = NULL;
    else {
        res->addr = calloc(strlen(addr) + 1, sizeof(char));
        strcpy(res->addr, addr);
    }
    if (phone == NULL) res->phone = NULL;
    else {
        res->phone = calloc(strlen(phone) + 1, sizeof(char));
        strcpy(res->phone, phone);
    }
    if (birthdate == NULL) res->birthdate = NULL;
    else {
        res->birthdate = calloc(strlen(birthdate) + 1, sizeof(char));
        strcpy(res->birthdate, birthdate);
    }
    return res;
}

struct ListData *copyListData(struct ListData *data) {
    struct ListData *res = calloc(1, sizeof(struct ListData));
    res->name = calloc(strlen(data->name) + 1, sizeof(char));
    res->surname = calloc(strlen(data->surname) + 1, sizeof(char));
    res->addr = calloc(strlen(data->addr) + 1, sizeof(char));
    res->birthdate = calloc(strlen(data->birthdate) + 1, sizeof(char));
    res->email = calloc(strlen(data->email) + 1, sizeof(char));
    res->phone = calloc(strlen(data->phone) + 1, sizeof(char));

    if (data->name != NULL) strcpy(res->name, data->name);
    if (data->surname != NULL) strcpy(res->surname, data->surname);
    if (data->birthdate != NULL) strcpy(res->birthdate, data->birthdate);
    if (data->phone != NULL) strcpy(res->phone, data->phone);
    if (data->email != NULL) strcpy(res->email, data->email);
    if (data->addr != NULL) strcpy(res->addr, data->addr);
    return res;
}

//int compareData(struct ListData *info1, struct ListData *info2) {
//    if (strcmp(info1->name, info2->name) != 0) return 0;
//    if (strcmp(info1->surname, info2->surname) != 0) return 0;
//    if (strcmp(info1->email, info2->email) != 0) return 0;
//    if (strcmp(info1->addr, info2->addr) != 0) return 0;
//    if (strcmp(info1->phone, info2->phone)) return 0;
//    if (strcmp(info1->birthdate, info2->birthdate)) return 0;
//    return 1;
//}

struct List *makeList(struct List *list) {
    if (list != NULL) return list;
    struct List *res = calloc(1, sizeof(struct List));
    res->first = NULL;
    res->last = NULL;
    return res;
}

struct ListNode *findNode(char *type, char *text, struct List *pointer) {
    struct ListNode *i;
    i = NULL;
    if (strcmp(type, "addr") == 0) {
        for (i = pointer->first; i != NULL; i = i->next) {
            if (strcmp(i->info->addr, text) == 0) return i;
        }
    } else if (strcmp(type, "surname") == 0) {
        for (i = pointer->first; i != NULL; i = i->next) {
            if (strcmp(i->info->surname, text) == 0) return i;
        }
    } else if (strcmp(type, "name") == 0) {
        for (i = pointer->first; i != NULL; i = i->next) {
            if (strcmp(i->info->name, text) == 0) return i;
        }
    } else if (strcmp(type, "email") == 0) {
        for (i = pointer->first; i != NULL; i = i->next) {
            if (strcmp(i->info->email, text) == 0) return i;
        }
    } else if (strcmp(type, "birthdate") == 0) {
        for (i = pointer->first; i != NULL; i = i->next) {
            if (strcmp(i->info->birthdate, text) == 0) return i;
        }
    } else if (strcmp(type, "phone") == 0) {
        for (i = pointer->first; i != NULL; i = i->next) {
            if (strcmp(i->info->phone, text) == 0) return i;
        }
    }
    return NULL;
}

struct List *listSort(char *type, struct List *n) {
    struct ListNode *rnr;
    struct ListData *swapper;
    if (n == NULL) return n;
    if (n->first == n->last) return n;

    if (strcmp(type, "addr") == 0) {
        int sorted = 1;
        while (1) {
            for (rnr = n->first; rnr->next != NULL; rnr = rnr->next) {
                sorted = 1;
                if (strcmp(rnr->info->addr, rnr->next->info->addr) > 0) {
                    sorted = 0;
                    swapper = rnr->info;
                    rnr->info = rnr->next->info;
                    rnr->next->info = swapper;
                }
            }
            if (sorted == 1) break;
        }
    } else if (strcmp(type, "phone") == 0) {
        int sorted = 1;
        while (1) {
            for (rnr = n->first; rnr->next != NULL; rnr = rnr->next) {
                sorted = 1;
                if (strcmp(rnr->info->phone, rnr->next->info->phone) > 0) {
                    sorted = 0;
                    swapper = rnr->info;
                    rnr->info = rnr->next->info;
                    rnr->next->info = swapper;
                }
            }
            if (sorted == 1) break;
        }
    } else if (strcmp(type, "name") == 0) {
        int sorted = 1;
        while (1) {
            for (rnr = n->first; rnr->next != NULL; rnr = rnr->next) {
                sorted = 1;
                if (strcmp(rnr->info->name, rnr->next->info->name) > 0) {
                    sorted = 0;
                    swapper = rnr->info;
                    rnr->info = rnr->next->info;
                    rnr->next->info = swapper;
                }
            }
            if (sorted == 1) break;
        }
    } else if (strcmp(type, "surname") == 0) {
        int sorted = 1;
        while (1) {
            for (rnr = n->first; rnr->next != NULL; rnr = rnr->next) {
                sorted = 1;
                if (strcmp(rnr->info->surname, rnr->next->info->surname) > 0) {
                    sorted = 0;
                    swapper = rnr->info;
                    rnr->info = rnr->next->info;
                    rnr->next->info = swapper;
                }
            }
            if (sorted == 1) break;
        }
    } else if (strcmp(type, "birthdate") == 0) {
        int sorted = 1;
        while (1) {
            for (rnr = n->first; rnr->next != NULL; rnr = rnr->next) {
                sorted = 1;
                if (strcmp(rnr->info->birthdate, rnr->next->info->birthdate) > 0) {
                    sorted = 0;
                    swapper = rnr->info;
                    rnr->info = rnr->next->info;
                    rnr->next->info = swapper;
                }
            }
            if (sorted == 1) break;
        }
    } else if (strcmp(type, "email") == 0) {
        int sorted = 1;
        while (1) {
            for (rnr = n->first; rnr->next != NULL; rnr = rnr->next) {
                sorted = 1;
                if (strcmp(rnr->info->email, rnr->next->info->email) > 0) {
                    sorted = 0;
                    swapper = rnr->info;
                    rnr->info = rnr->next->info;
                    rnr->next->info = swapper;
                }
            }
            if (sorted == 1) break;
        }
    }

    return n;
}

struct List *deleteNode(struct List *x, struct ListNode *n) {
    if (n == NULL) return x;
    if (n->prev == NULL) {
        if (n->next == NULL) {
            free(n->info->surname);
            free(n->info->phone);
            free(n->info->name);
            free(n->info->email);
            free(n->info->addr);
            free(n->info->birthdate);
            free(n->info);
            free(n);
            x->first = NULL;
            x->last = NULL;
            return x;
        }
        n->next->prev = NULL;
        x->first = x->first->next;
        free(n->info->surname);
        free(n->info->phone);
        free(n->info->name);
        free(n->info->email);
        free(n->info->addr);
        free(n->info->birthdate);
        free(n->info);
        free(n);
    } else if (n->next == NULL) {
        n->prev->next = NULL;
        x->last = x->last->prev;
        free(n->info->surname);
        free(n->info->phone);
        free(n->info->name);
        free(n->info->email);
        free(n->info->addr);
        free(n->info->birthdate);
        free(n->info);
        free(n);
    } else {
        n->next->prev = n->prev;
        n->prev->next = n->next;
        free(n->info->surname);
        free(n->info->phone);
        free(n->info->name);
        free(n->info->email);
        free(n->info->addr);
        free(n->info->birthdate);
        free(n->info);
        free(n);
    }
    return x;
}

void addToList(struct ListData *info, struct List *pointer) {
    if (pointer->first == NULL) {
        pointer->first = calloc(1, sizeof(struct ListNode));
        pointer->last = pointer->first;
        pointer->first->info = copyListData(info);
        pointer->first->next = NULL;
        pointer->first->prev = NULL;
        pointer->last->next = NULL;
        pointer->last->prev = NULL;
    } else {
        pointer->last->next = calloc(1, sizeof(struct ListNode));
        pointer->last->next->prev = pointer->last;
        pointer->last->next->next = NULL;
        pointer->last->next->info = copyListData(info);
        pointer->last = pointer->last->next;
        pointer->last->next = NULL;
    }
}

struct List *removeFromList(char *type, char *text, struct List *pointer) {
    struct ListNode *del;
    del = findNode(type, text, pointer);
    if (del == NULL) return pointer;
    pointer = deleteNode(pointer, del);
    return pointer;
}

struct List *deleteList(struct List *pointer) {
    struct ListNode *runner;
    struct ListNode *tmp;
    runner = pointer->first;
    while (runner != NULL) {
        tmp = runner;
        if (runner->next == NULL) {
            free(runner);
            return NULL;
        }
        runner = runner->next;
        free(tmp->info->addr);
        free(tmp->info->birthdate);
        free(tmp->info->email);
        free(tmp->info->name);
        free(tmp->info->phone);
        free(tmp->info->surname);
        free(tmp->info);
        free(tmp);
        tmp = NULL;
    }
    runner = NULL;
    free(pointer);
    pointer = NULL;
    return pointer;
}

struct ListData *getListData(struct List *list, char *type, char *in) {
    struct ListNode *res = NULL;
    res = findNode(type, in, list);
    if (res != NULL) return copyListData(res->info);
    else return NULL;
}
