#include"queue.h"

int is_full(struct qnode* node, int size){
	if(node[size-1].pid!=-1) return 1;
	return 0;
}

void init_queue(struct qnode* node, int size){
	for(int i=0; i<size; i++){
		node[i].pid=-1;
		node[i].semnum=-1;
	}
}

int queue_size(struct qnode* node, int size){
	int cntr=0;
	for(int i=0; i<size; i++){
		if(node[i].pid==-1) break;
		cntr++;
	}
	return cntr;
}

void add_to_queue(struct qnode in, struct qnode *node, int size){
	for(int i=0; i<size; i++){
		if(node[i].pid==-1){
			if(i==size-1) break;
			node[i].pid=in.pid;
			node[i].semnum=in.semnum;
			break;
		}
	}
}

void delete_first(struct qnode *node, int size){
	for(int i=0; i<size-1; i++){
		if(node[i].pid==-1){
			node[i].semnum=-1;
			break;
		}
		node[i].pid=node[i+1].pid;
		node[i].semnum=node[i+1].semnum;
	}
}

struct qnode get_value(struct qnode *node, int size){
	if(node[0].pid==-1) return node[0];
	struct qnode res;
	res.pid=node[0].pid;
	res.semnum=node[0].semnum;
	delete_first(node, size);
	return res;
}


