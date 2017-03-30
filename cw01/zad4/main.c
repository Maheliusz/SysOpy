#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/addressBook.h"
#include <time.h>
#include <sys/times.h>


struct ListData *listparse(char *data) {
    int length[6];
    int i, x;
    for (i = 0; i < 6; i++) length[i] = 0;
    for (i = 0, x = 0; i < strlen(data); i++) {
        if (data[i] != ';' && data[i] != '\0' && data[i] != '\n') length[x]++;
        else x++;
    }
    char *tmp[6];
    for (i = 0; i < 6; i++) {
        tmp[i] = calloc(length[i] + 1, sizeof(char));
    }
    for (i = 0, x = 0; i < 6; i++) {
        int j;
        for (j = 0; j < length[i]; j++) {
            tmp[i][j] = data[x++];
            if (data[x] == ';' || data[x] == '\n' || data[i] == '\0') {
                tmp[i][j + 1] = '\0';
                x++;
            }
        }
    }
    struct ListData *c = createData(tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
    for (i = 0; i < 6; i++) free(tmp[i]);
    return c;
}

struct treeData *treeparse(char *data) {
    int length[6];
    int i, x;
    for (i = 0; i < 6; i++) length[i] = 0;
    for (i = 0, x = 0; i < strlen(data); i++) {
        if (data[i] != ';' && data[i] != '\0' && data[i] != '\n') length[x]++;
        else x++;
    }
    char *tmp[6];
    for (i = 0; i < 6; i++) {
        tmp[i] = calloc(length[i] + 1, sizeof(char));
    }
    for (i = 0, x = 0; i < 6; i++) {
        int j;
        for (j = 0; j < length[i]; j++) {
            tmp[i][j] = data[x++];
            if (data[x] == ';' || data[x] == '\n' || data[i] == '\0') {
                tmp[i][j + 1] = '\0';
                x++;
            }
        }
    }
    struct treeData *c = packData(tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
    for (i = 0; i < 6; i++) free(tmp[i]);
    return c;
}

struct ListData **readlist() {
    FILE *in = fopen("./data1.txt", "r");
    if (in == NULL) perror("fopen");
    struct ListData **c = NULL;
    if ((in) != NULL) {
        c = calloc(1000, sizeof(struct ListData *));
        int i;
        char tmp[200];
        fscanf(in, "%[^\n]\n", tmp);
        for (i = 0; i < 1000; i++) {
            fscanf(in, "%[^\n]\n", tmp);
            c[i] = listparse(tmp);
        }
        fclose(in);
    }
    return c;
}

struct treeData **readtree() {
    FILE *in = fopen("./data1.txt", "r");
    if (in == NULL) perror("fopen");
    struct treeData **c = NULL;
    if ((in) != NULL) {
        c = calloc(1000, sizeof(struct treeData *));
        int i;
        char tmp[200];
        fscanf(in, "%[^\n]\n", tmp);
        for (i = 0; i < 1000; i++) {
            fscanf(in, "%[^\n]\n", tmp);
            c[i] = treeparse(tmp);
        }
        fclose(in);
    }
    return c;
}

int main(void) {
    clock_t _rStart, _rEnd;
    struct tms _start, _end;
    struct ListData **lists;
    struct treeData **trees;
    struct Tree *tree = NULL;
    struct List *list = NULL;
    double rlcreate[2];
    double rlinsertlist[1000];
    double rlinserttree[999];
    double rldelete[4];
    double rlsearch[4];
    double rlsort[2];
    double usrcreate[2];
    double usrinsertlist[1000];
    double usrinserttree[999];
    double usrdelete[4];
    double usrsearch[4];
    double usrsort[2];
    double syscreate[2];
    double sysinsertlist[1000];
    double sysinserttree[999];
    double sysdelete[4];
    double syssearch[4];
    double syssort[2];
    times(&_start);
    _rStart = clock();
    tree = createTree(trees[0], "name", tree);
    _rEnd = clock();
    times(&_end);
    rlcreate[1] = difftime(_rEnd, _rStart);
    usrcreate[1] = difftime(_end.tms_utime, _start.tms_utime);
    syscreate[1] = difftime(_end.tms_stime, _start.tms_stime);
    times(&_start);
    printf("Tree creation times: real, user, system: %f \t %f \t %f \n", rlcreate[1], usrcreate[1], syscreate[1]);
    _rStart = clock();
    list = makeList(list);
    _rEnd = clock();
    times(&_end);
    rlcreate[0] = difftime(_rEnd, _rStart);
    printf("List creation times: real, user, system: %f \t %f \t %f \n", rlcreate[0], usrcreate[0], syscreate[0]);
    usrcreate[0] = difftime(_end.tms_utime, _start.tms_utime);
    syscreate[0] = difftime(_end.tms_stime, _start.tms_stime);
    for (int i = 0; i < 1000; i++) {
            times(&_start);
        _rStart = clock();
        addToList(lists[i], list);
        _rEnd = clock();
            times(&_end);
        rlinsertlist[i] = difftime(_rEnd, _rStart);
            usrinsertlist[i] = difftime(_end.tms_utime, _start.tms_utime);
            sysinsertlist[i] = difftime(_end.tms_stime, _start.tms_stime);
    }
    for (int i = 0; i < 999; i++) {
            times(&_start);
        _rStart = clock();
        tree = insertIntoTree(tree, trees[i]);
        _rEnd = clock();
            times(&_end);
        rlinserttree[i] = difftime(_rEnd, _rStart);
            usrinsertlist[i] = difftime(_end.tms_utime, _start.tms_utime);
            sysinsertlist[i] = difftime(_end.tms_stime, _start.tms_stime);
    }
    printf("List insert optimistic times: real, user, system: %f \t %f \t %f \n",
           rlinsertlist[0], usrinsertlist[0], sysinsertlist[0]);
    printf("List insert pessimistic times: real, user, system: %f \t %f \t %f \n",
           rlinsertlist[999], usrinsertlist[999], sysinsertlist[999]);
    printf("Tree insert optimistic times: real, user, system: %f \t %f \t %f \n",
           rlinserttree[0], usrinserttree[0], sysinserttree[0]);
    printf("Tree insert pessimistic times: real, user, system: %f \t %f \t %f \n",
           rlinserttree[998], usrinserttree[998], sysinserttree[998]);
        times(&_start);
    _rStart = clock();
    struct ListData *tmp = getListData(list, "name", "Stella");
    _rEnd = clock();
        times(&_end);
    rlsearch[0] = difftime(_rEnd, _rStart);
    usrsearch[0] = difftime(_end.tms_utime, _start.tms_utime);
    syssearch[0] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    tmp = getListData(list, "name", "Nieistnieje");
    _rEnd = clock();
        times(&_end);
    rlsearch[1] = difftime(_rEnd, _rStart);
        usrsearch[1] = difftime(_end.tms_utime, _start.tms_utime);
    syssearch[1] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    struct treeData *rnr = getTreeData(tree, "Stella");
    _rEnd = clock();
        times(&_end);
    rlsearch[2] = difftime(_rEnd, _rStart);
        usrsearch[2] = difftime(_end.tms_utime, _start.tms_utime);
    syssearch[2] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    rnr = getTreeData(tree, "Nieistnieje");
    _rEnd = clock();
        times(&_end);
    rlsearch[3] = difftime(_rEnd, _rStart);
        usrsearch[3] = difftime(_end.tms_utime, _start.tms_utime);
    syssearch[3] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    printf("List search optimistic times: real, user, system: %f \t %f \t %f \n", rlsearch[0], usrsearch[0], syssearch[0]);
    printf("List search pessimistic times: real, user, system: %f \t %f \t %f \n",
           rlsearch[1], usrsearch[1], syssearch[1]);
    printf("Tree search optimistic times: real, user, system: %f \t %f \t %f \n", rlsearch[2], usrsearch[2], syssearch[2]);
    printf("Tree search pessimistic times: real, user, system: %f \t %f \t %f \n",
           rlsearch[3], usrsearch[3], syssearch[3]);
    _rStart = clock();
    list = removeFromList("name", "Stella", list);
    _rEnd = clock();
        times(&_end);
    rldelete[0] = difftime(_rEnd, _rStart);
        usrdelete[0] = difftime(_end.tms_utime, _start.tms_utime);
    sysdelete[0] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    list = removeFromList("name", "Nieistnieje", list);
    _rEnd = clock();
        times(&_end);
    rldelete[1] = difftime(_rEnd, _rStart);
        usrdelete[1] = difftime(_end.tms_utime, _start.tms_utime);
    sysdelete[1] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    tree = delete(tree, "Stella");
    _rEnd = clock();
        times(&_end);
    rldelete[2] = difftime(_rEnd, _rStart);
        usrdelete[2] = difftime(_end.tms_utime, _start.tms_utime);
    sysdelete[2] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    tree = delete(tree, "XYZ");
    _rEnd = clock();
        times(&_end);
    rldelete[3] = difftime(_rEnd, _rStart);
        usrdelete[3] = difftime(_end.tms_utime, _start.tms_utime);
    sysdelete[3] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    printf("List delete optimistic times: real, user, system: %f \t %f \t %f \n", rldelete[0], usrdelete[0], sysdelete[0]);
    printf("List delete pessimistic times: real, user, system: %f \t %f \t %f \n",
           rldelete[1], usrdelete[1], sysdelete[1]);
    printf("Tree delete optimistic times: real, user, system: %f \t %f \t %f \n", rldelete[2], usrdelete[2], sysdelete[2]);
    printf("Tree delete pessimistic times: real, user, system: %f \t %f \t %f \n",
           rldelete[3], usrdelete[3], sysdelete[3]);
    _rStart = clock();
    list = listSort("phone", list);
    _rEnd = clock();
        times(&_end);
    rlsort[0] = difftime(_rEnd, _rStart);
        usrsort[0] = difftime(_end.tms_utime, _start.tms_utime);
    syssort[0] = difftime(_end.tms_stime, _start.tms_stime);
        times(&_start);
    _rStart = clock();
    tree = treeSort(tree, "phone");
    _rEnd = clock();
        times(&_end);
    rlsort[1] = difftime(_rEnd, _rStart);
        usrsort[1] = difftime(_end.tms_utime, _start.tms_utime);
    syssort[1] = difftime(_end.tms_stime, _start.tms_stime);
    printf("List sort times: real, user, system: %f \t %f \t %f \n", rlsort[0], usrsort[0], syssort[0]);
    printf("Tree sort times: real, user, system: %f \t %f \t %f \n", rlsort[1], usrsort[1], syssort[1]);
	getchar();
    return 0;
}
