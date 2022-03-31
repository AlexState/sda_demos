#include <stdlib.h>
#include "Queue.h"
#include "utils.h"

queue_t * q_create(unsigned int data_size, unsigned int max_size){

	queue_t* q = calloc(1, sizeof(*q));
	//DIE
	
	q->data_size = data_size;
	q->max_size = max_size;
	q->size = 0u;
	q->read_idx = 0u;
	q->write_idx = 0u;
	q->buff = calloc(max_size, sizeof(void*));
	return q;
}


unsigned int q_get_size(queue_t *q){
	if(q == NULL) return -1;
	return q->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int q_is_empty(queue_t *q){

	if(q->size == 0) return 1;
 	return 0;
}

void * q_front(queue_t *q){

	if (!q->size)
		return NULL;

	return q->buff[q->read_idx];
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool q_dequeue(queue_t *q){

	if (!q->size)
		return false;

	free(q->buff[q->read_idx]);
	q->buff[q->read_idx] = NULL;

	q->read_idx++;
	q->read_idx %= q->max_size;

	q->size--;
	return true;
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool q_enqueue(queue_t *q, void *new_data){

	if (q->size == q->max_size)
		return false;

	q->buff[q->write_idx] = calloc(1, q->data_size);
	memcpy(q->buff[q->write_idx], new_data, q->data_size);
	
	q->write_idx++;
	q->write_idx %= q->max_size;

	q->size ++;
	return true;
}

void q_clear(queue_t *q){
	while(q_dequeue(q)){
		//nothing to do.
	}

}

void q_free(queue_t *q){
	q_clear(q);
	
	free(q->buff);
	q->buff = NULL;

	free(q);
}
