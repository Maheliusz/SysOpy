#ifndef LAB1_LISTADDRESS_H
#define LAB1_LISTADDRESS_H

struct ListData {
    char *name;
    char *surname;
    char *birthdate;
    char *email;
    char *phone;
    char *addr;
};

struct ListNode {
    struct ListData *info;
    struct ListNode *next;
    struct ListNode *prev;
};

struct List {
    struct ListNode *first;
    struct ListNode *last;
};

struct ListData *createData(char *name, char *surname, char *birthdate, char *email, char *phone, char *addr);

//int compareData(struct ListData *info1, struct ListData *info2);

struct List *makeList(struct List *list);

struct List *listSort(char *type, struct List *n);

void addToList(struct ListData *info, struct List *pointer);

struct List *removeFromList(char *type, char *text, struct List *pointer);

struct List *deleteList(struct List *pointer);

struct ListData *getListData(struct List *list, char *type, char *in);

#endif
