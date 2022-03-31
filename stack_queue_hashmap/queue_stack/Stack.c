#include <stdlib.h>

#include "Stack.h"
#include "utils.h"

stack_t * st_create(unsigned int data_size){

	stack_t* st = calloc(1, sizeof(*st));
	//DUE()...
	st->list = ll_create(data_size);
	return st;
}

unsigned int st_get_size(stack_t *st)
{
	if(st == NULL) return -1;
	return st->list->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int st_is_empty(stack_t *st){
	if(st->list->size == 0) return 1;
 	return 0;
}

void * st_peek(stack_t *st){

	return st->list->head->data;

}

void st_pop(stack_t *st){

	ll_node_t* toDel = ll_remove_nth_node(st->list, 0);
	free(toDel->data);
	free(toDel);

}

void st_push(stack_t *st, void *new_data){

	ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t *st)
{
	while(!st_is_empty(st)){
		st_pop(st);
	}

}

void st_free(stack_t *st){

	st_clear(st);
	free(st);
	st = NULL;

}
