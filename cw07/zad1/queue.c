#include"queue.h"

int queue_size(struct qnode* node){
	if(node==NULL) return 0;
	int cntr=1;
	struct qnode *rnr=node;
	while(rnr->next!=NULL){
		cntr++;
		rnr=rnr->next;
	}
	return cntr;
}

struct qnode *add_to_queue(int val, struct qnode *node){
	if(node==NULL){
		node = calloc(1, sizeof(struct qnode));
		node->next=NULL;
		node->val=val;
	}else{
		struct qnode *rnr=node;
		while(rnr->next!=NULL) rnr=rnr->next;
		rnr->next = calloc(1, sizeof(struct qnode));
		rnr=rnr->next;
		rnr->next=NULL;
		rnr->val=val;
	}
	return node;
}

int get_value(struct qnode *node){
	if(node==NULL) return -1;
	int res=node->val;
	node = delete_first(node);
	return res;
}

struct qnode *delete_first(struct qnode *node){
	struct qnode *rnr=node;
	node=node->next;
	free(rnr);
	return node;
}
