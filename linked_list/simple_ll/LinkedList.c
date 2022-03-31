//
// Created by Alex on 11/6/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t* ll_create(unsigned int data_size){

    linked_list_t* instance_ll = (linked_list_t*)malloc(sizeof(linked_list_t));
    DIE(instance_ll == NULL, "linked_list malloc");
    
    instance_ll->head = NULL;
    instance_ll->size = 0;
    instance_ll->data_size=data_size;
    return instance_ll;
}

/**
 * @brief Create a node object. Alloc memory for the struct and data_size bytes
 *
 * @param new_data      pointer to the data to be copied into the node
 * @param data_size     the number of bytes to be copied
 * @return ll_node_t*   the newly created node
 */
static ll_node_t* create_node(const void* new_data, unsigned int data_size){
    ll_node_t* node = (ll_node_t*)malloc(sizeof (ll_node_t));
    DIE(node == NULL, "node malloc");
    
    
    node->next = NULL;
    node->data = malloc(data_size);
    memcpy(node->data, new_data, data_size);
    return node;

}

/**
 * @brief Get the nth node from the list. 
 * If n is larger than list->size - 1, return list->size - 1.
 *
 * @param n             the position to return
 * @param list          the list
 * @return ll_node_t*   the nth node from the list
 */
static ll_node_t* get_nth_node(unsigned int n, linked_list_t *list){
    ll_node_t *node = list->head;

    for (int i = 0; i < n; ++i)
        node = node->next;
    return node;
}


/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data){

    if (!list || n <0)
        return;
    if(n > list->size)
        n = list->size ;

    ll_node_t* new_node =  (ll_node_t*)create_node(new_data, list->data_size);


    if (n == 0){ 
        new_node->next = list->head;
        list->head = new_node;
    }
    else{ 
        ll_node_t * prev_node = NULL;
        prev_node = get_nth_node(n - 1, list);
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }
    list->size ++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n){

    ll_node_t *prev = NULL;
    ll_node_t* removed = NULL;

    if (list==NULL || list->size == 0 || n < 0)
        return NULL;


    if (n == 0){ 
        removed = list->head;
        list->head = list->head->next;

    }
    else{ 
        prev = get_nth_node(n - 1, list);
        removed = prev->next;
        prev->next = prev->next->next;

    }
    list->size--;
    return removed;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(linked_list_t* list){

   if (list == NULL) {
        return -1;
    }
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t** pp_list){

    if (!pp_list || !*pp_list)
        return;
    ll_node_t * node = NULL;

    while((*pp_list)->size){

        node = ll_remove_nth_node((*pp_list), 0);
        free(node->data);
        free(node);
    }
    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t* list){
    ll_node_t *node = list->head;
    // int(data): *(int *)node->data
    int i;
    for(i = 0; i < list->size; ++i){
        printf("%d ",*(int *)node->data );
        node = node->next;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t* list){

    ll_node_t *node = list->head;
    // int(data): *(int *)node->data
    int i;
    for(i = 0; i < list->size; ++i){
        printf("%s ", (const char *)node->data );
        node = node->next;
    }
    printf("\n");
}
