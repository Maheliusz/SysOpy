#include"queue.h"

void generate_path(char *res){
	srand(time(NULL));
	res[0]='/';
	for(int i=1; i<7; i++){
		res[i]=(char)(rand()%26)+97;
	}
	res[7]='\0';
}

int is_full(struct qnode* node){
	if(node[MAXSEM-1].pid!=-1) return 1;
	return 0;
}

int queue_size(struct qnode* node){
	int cntr=0;
	for(int i=0; i<MAXSEM; i++){
		if(node[i].pid==-1) break;
		cntr++;
	}
	return cntr;
}

void add_to_queue(struct qnode in, struct qnode *node){
	for(int i=0; i<MAXSEM; i++){
		if(node[i].pid==-1){
			if(i==MAXSEM-1) break;
			node[i].pid=in.pid;
			strcpy(node[i].semnum, in.semnum);
			break;
		}
	}
}

void delete_first(struct qnode *node){
	int i=0;
	for(; i<MAXSEM-1; i++){
		if(node[i].pid==-1) break;
		node[i].pid=node[i+1].pid;
		strcpy(node[i].semnum, node[i+1].semnum);
	}
	node[i].pid=-1;
}

struct qnode get_value(struct qnode *node){
	if(node[0].pid==-1) return node[0];
	struct qnode res;
	res.pid=node[0].pid;
	strcpy(res.semnum, node[0].semnum);
	delete_first(node);
	return res;
}


