#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CircularDoublyLinkedList.h"

#include "utils.h"

/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
doubly_linked_list_t* dll_create(unsigned int data_size){
    doubly_linked_list_t* instance_dll = (doubly_linked_list_t*)malloc(sizeof(doubly_linked_list_t));
    DIE(instance_dll == NULL, "linked_list malloc");    
    
    instance_dll->head = NULL;
    instance_dll->size = 0;
    instance_dll->data_size=data_size;
    return instance_dll;
}


/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
dll_node_t* dll_get_nth_node(doubly_linked_list_t* list, unsigned int n){
    if(n < 0){
        //error
        exit(1);
    }
    if(n > list->size)
        n = n % list->size;

    dll_node_t* node = list->head;
    for (int i = 0; i < n; ++i)
        node = node->next;
    return node;
}




static dll_node_t* create_node(const void* new_data, unsigned int data_size){
    dll_node_t* node = (dll_node_t*)malloc(sizeof (dll_node_t));
    DIE(node == NULL, "NODE malloc");       

    node->next = node;
    node->prev = node;
    node->data = malloc(data_size);
    memcpy(node->data, new_data, data_size);
    return node;
}



/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data){
 
    if (!list || n <0)
        return;


    dll_node_t* new_node =  create_node(data, list->data_size);

        if(list->size == 0){
            list->head = new_node;
        }else{
            dll_node_t * current = NULL;
            current = dll_get_nth_node(list, n);

            new_node->next = current;
            new_node->prev = current->prev;

            current->prev->next = new_node;
            current->prev = new_node;

            if(n ==0) list->head = new_node;
        }

    list->size ++;
}


/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
dll_node_t* dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n){

    dll_node_t* current = NULL;
    dll_node_t* removed = NULL;

    if (list==NULL || list->size == 0 || n < 0)
        return NULL;

    removed = dll_get_nth_node(list, n);
    current = removed;
        
    //update links
    current->prev->next = current->next;
    current->next->prev = current->prev;
    current=current->next;

    //isolate removed node
    removed->next = removed;
    removed->prev = removed;
    list->size --;
    
    if(removed == list->head) list->head =current;
    if(list->size == 0) list->head = NULL;
    
    return  removed;
}



/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int dll_get_size(doubly_linked_list_t* list){

   if (list == NULL) {
        return -1;
    }
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void dll_free(doubly_linked_list_t** pp_list){
    if (!pp_list || !(*pp_list) )
        return;
    dll_node_t* node = NULL;

    while((*pp_list)->size){

        node = dll_remove_nth_node((*pp_list), 0);
        free(node->data);
        free(node);
    }
    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void dll_print_int_list(doubly_linked_list_t* list){

    dll_node_t *node = list->head;
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
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void dll_print_string_list(doubly_linked_list_t* list){
    dll_node_t *node = list->head;
    // int(data): *(int *)node->data
    int i;
    for(i = 0; i < list->size; ++i){
        printf("%s ", (const char *)node->data );
        node = node->next;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void dll_print_ints_left_circular(dll_node_t* start){

    // crt = start->prev
    // while (crt != start) {...}
    dll_node_t* current = start;
    do{
        printf("%d ",*(int *)current->data );
        current = current->next;
    }while(current != start);

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void dll_print_ints_right_circular(dll_node_t* start){

    dll_node_t* current = start;
    do{
        printf("%d ",*(int *)current->data );
        current = current->prev;
    }while(current != start);
    printf("\n");
}


void dll_remove_duplicates(doubly_linked_list_t *list){
    // a <-> a <-> a <-> b <-> c <-> c
    // cat timp *crt->data != *crt->next->data: remove(crt->next)
    dll_node_t* current = list->head;
    if(list->size == 1) return;

    do{
        if (current->data == current->next->data) {
            dll_node_t * removed = current->next;
            //update links
            current->next = current->next->next;
            current->next->prev = current;
            //freee
            free(removed->data);
            free(removed);
            //next node
            current = current->next;
        } else {
            current = current->next;
        }
    }while(current != list->head);
}
