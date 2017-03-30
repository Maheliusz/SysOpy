#ifndef LAB1_TREEADDRESS_H
#define LAB1_TREEADDRESS_H

struct treeData {
    char *name;
    char *surname;
    char *birthdate;
    char *email;
    char *phone;
    char *addr;
};

struct TreeNode {
    struct treeData *info;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
};

struct Tree {
    struct TreeNode *top;
    char *type;
};


struct treeData *packData(char *name, char *surname, char *birthdate, char *email, char *phone, char *addr);

struct Tree *createTree(struct treeData *info, char *type, struct Tree *tree);

void deleteTree(struct Tree *tree);

int compare(struct treeData *info1, struct treeData *info2, char *type);

struct Tree *insertIntoTree(struct Tree *tree, struct treeData *info);

struct Tree *delete(struct Tree *tree, char *in);

struct treeData *getTreeData(struct Tree *tree, char *in);

struct Tree *treeSort(struct Tree *tree, char *type);

#endif
