#include <string.h>
#include <stdlib.h>
#include "treeAddressBook.h"


struct treeData *packData(char *name, char *surname, char *birthdate, char *email, char *phone, char *addr) {
    struct treeData *res = calloc(1, sizeof(struct treeData));
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

struct treeData *CopyTreeData(struct treeData *data) {
    struct treeData *res = calloc(1, sizeof(struct treeData));
    if (data->name != NULL) res->name = calloc(strlen(data->name) + 1, sizeof(char));
    else res->name = NULL;
    if (data->surname != NULL) res->surname = calloc(strlen(data->surname) + 1, sizeof(char));
    else res->surname = NULL;
    if (data->birthdate != NULL) res->addr = calloc(strlen(data->addr) + 1, sizeof(char));
    else res->birthdate = NULL;
    if (data->phone != NULL) res->birthdate = calloc(strlen(data->birthdate) + 1, sizeof(char));
    else res->phone = NULL;
    if (data->email != NULL) res->email = calloc(strlen(data->email) + 1, sizeof(char));
    else res->email = NULL;
    if (data->addr != NULL) res->phone = calloc(strlen(data->phone) + 1, sizeof(char));
    else res->addr = NULL;

    if (data->name != NULL) strcpy(res->name, data->name);
    if (data->surname != NULL) strcpy(res->surname, data->surname);
    if (data->birthdate != NULL) strcpy(res->birthdate, data->birthdate);
    if (data->phone != NULL) strcpy(res->phone, data->phone);
    if (data->email != NULL) strcpy(res->email, data->email);
    if (data->addr != NULL) strcpy(res->addr, data->addr);
    return res;
}

struct Tree *createTree(struct treeData *info, char *type, struct Tree *tree) {
    if (tree != NULL) return tree;
    struct Tree *res = NULL;
    if (info == NULL) {
        return res;
    }
    res = calloc(1, sizeof(struct Tree));
    res->top = calloc(1, sizeof(struct TreeNode));
    res->top->info = CopyTreeData(info);
    res->top->left = NULL;
    res->top->right = NULL;
    res->top->parent = NULL;
    res->type = calloc(strlen(type) + 1, sizeof(char));
    strcpy(res->type, type);
    return res;
}

void removeAllNodes(struct TreeNode *node) {
    if (node == NULL) return;
    if (node->left != NULL) removeAllNodes(node->left);
    if (node->right != NULL) removeAllNodes(node->right);
    free(node->info->surname);
    free(node->info->phone);
    free(node->info->name);
    free(node->info->email);
    free(node->info->addr);
    free(node->info->birthdate);
    free(node->info);
    free(node);
    node = NULL;
}

void deleteTree(struct Tree *tree) {
    if (tree == NULL) return;
    if (tree->top == NULL) {
        free(tree);
        return;
    }
    removeAllNodes(tree->top);
    free(tree);
}

int compare(struct treeData *info1, struct treeData *info2, char *type) {
    if (strcmp(type, "name") == 0) {
        return strcmp(info1->name, info2->name);
    } else if (strcmp(type, "addr") == 0) {
        return strcmp(info1->addr, info2->addr);
    } else if (strcmp(type, "surname") == 0) {
        return strcmp(info1->surname, info2->surname);
    } else if (strcmp(type, "email") == 0) {
        return strcmp(info1->email, info2->email);
    } else if (strcmp(type, "birthdate") == 0) {
        return strcmp(info1->birthdate, info2->birthdate);
    } else if (strcmp(type, "phone") == 0) {
        return strcmp(info1->phone, info2->phone);
    }
    return 0;
}

void insertIntoNode(struct TreeNode *node, struct treeData *info, char *type) {
    struct TreeNode *rnr = node;
    if (node == NULL) {
        node = calloc(1, sizeof(struct TreeNode));
        node->info = CopyTreeData(info);
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;
    }
    while (1) {
        if (compare(rnr->info, info, type) >= 0) {
            if (rnr->left == NULL) {
                rnr->left = calloc(1, sizeof(struct TreeNode));
                rnr->left->left = NULL;
                rnr->left->right = NULL;
                rnr->left->parent = rnr;
                rnr->left->info = CopyTreeData(info);
                return;
            } else {
                rnr = rnr->left;
            }
        } else {
            if (rnr->right == NULL) {
                rnr->right = calloc(1, sizeof(struct TreeNode));
                rnr->right->left = NULL;
                rnr->right->right = NULL;
                rnr->right->parent = rnr;
                rnr->right->info = CopyTreeData(info);
                return;
            } else {
                rnr = rnr->right;
            }
        }
    }
}

struct Tree *insertIntoTree(struct Tree *tree, struct treeData *info) {
    if (tree == NULL) {
        tree = createTree(info, tree->type, tree);
        return tree;
    } else {
        insertIntoNode(tree->top, info, tree->type);
        return tree;
    }
}

struct Tree *addToTree(struct Tree *tree, struct treeData *info, char* type){
    if (tree == NULL) {
        tree = createTree(info, type, tree);
        return tree;
    } else {
        insertIntoNode(tree->top, info, tree->type);
        return tree;
    }
}

struct Tree *deleteTreeNode(struct TreeNode *node, struct Tree *tree) {
    struct TreeNode *rnr = node;
    int turnedLeft = 0;
    if (node == NULL) return tree;
    while (1) {
        if (rnr->right != NULL) {
            free(rnr->info->birthdate);
            free(rnr->info->addr);
            free(rnr->info->email);
            free(rnr->info->name);
            free(rnr->info->surname);
            free(rnr->info->phone);
            free(rnr->info);
            rnr->info = CopyTreeData(rnr->right->info);
            rnr = rnr->right;
            turnedLeft = 0;
        } else if (rnr->left != NULL) {
            free(rnr->info->birthdate);
            free(rnr->info->addr);
            free(rnr->info->email);
            free(rnr->info->name);
            free(rnr->info->surname);
            free(rnr->info->phone);
            free(rnr->info);
            rnr->info = CopyTreeData(rnr->left->info);
            rnr = rnr->left;
            turnedLeft = 1;
//        } else if (rnr->parent == NULL) {
//            free(rnr->info->surname);
//            free(rnr->info->phone);
//            free(rnr->info->name);
//            free(rnr->info->email);
//            free(rnr->info->addr);
//            free(rnr->info->birthdate);
//            free(rnr->info);
//            free(rnr);
//            rnr = NULL;
//            return NULL;
        } else {
            if (turnedLeft && rnr->parent != NULL) {
                rnr->parent->left = NULL;
            } else if (!turnedLeft && rnr->parent != NULL) {
                rnr->parent->right = NULL;
            }
            free(rnr->info->surname);
            free(rnr->info->phone);
            free(rnr->info->name);
            free(rnr->info->email);
            free(rnr->info->addr);
            free(rnr->info->birthdate);
            free(rnr->info);
            free(rnr);
            return tree;
        }
    }
}

struct TreeNode *find(struct Tree *tree, char *in) {
    struct TreeNode *rnr = tree->top;
    struct treeData *info;
    if (strcmp(tree->type, "name") == 0) {
        info = packData(in, NULL, NULL, NULL, NULL, NULL);
    } else if (strcmp(tree->type, "surname") == 0) {
        info = packData(NULL, in, NULL, NULL, NULL, NULL);
    } else if (strcmp(tree->type, "phone") == 0) {
        info = packData(NULL, NULL, NULL, NULL, in, NULL);
    } else if (strcmp(tree->type, "birthdate") == 0) {
        info = packData(NULL, NULL, in, NULL, NULL, NULL);
    } else if (strcmp(tree->type, "email") == 0) {
        info = packData(NULL, NULL, NULL, in, NULL, NULL);
    } else if (strcmp(tree->type, "addr") == 0) {
        info = packData(NULL, NULL, NULL, NULL, NULL, in);
    } else return NULL;
    while (1) {
        if (rnr == NULL) {
            free(rnr);
            return NULL;
        }
        if (compare(rnr->info, info, tree->type) == 0) {
            return rnr;
        } else if (compare(rnr->info, info, tree->type) > 0) {
            rnr = rnr->right;
        } else {
            rnr = rnr->left;
        }
    }
}


struct Tree *addAndRemove(struct Tree *sorted, char *type, struct TreeNode *node) {
    sorted = addToTree(sorted, node->info, type);
    if (node->left != NULL) sorted = addAndRemove(sorted, sorted->type, node->left);
    if (node->right != NULL) sorted = addAndRemove(sorted, sorted->type, node->right);
    free(node->info->surname);
    free(node->info->phone);
    free(node->info->name);
    free(node->info->email);
    free(node->info->addr);
    free(node->info->birthdate);
    free(node->info);
    free(node);
    return sorted;
}

struct Tree *treeSort(struct Tree *tree, char *type) {
    if (tree == NULL) return NULL;
    struct Tree *res = NULL;
    res = addAndRemove(res, type, tree->top);
    return res;
}

struct Tree *delete(struct Tree *tree, char *in) {
    struct TreeNode *res = NULL;
    res = find(tree, in);
    if (res == NULL) {
        free(res);
        return tree;
    }
    tree = deleteTreeNode(res, tree);
    tree = treeSort(tree, tree->type);
    return tree;
}

struct treeData *getTreeData(struct Tree *tree, char *in) {
    struct TreeNode *res = NULL;
    res = find(tree, in);
    if (res == NULL) {
        free(res);
        return NULL;
    }
    return CopyTreeData(res->info);
}
