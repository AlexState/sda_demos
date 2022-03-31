#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "Hashtable.h"

#define MAX_BUCKET_SIZE 64

/*
 * Functii de comparare a cheilor:
 */
int compare_function_ints(void *a, void *b){
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_strings(void *a, void *b){
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int hash_function_int(void *a){
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_string(void *a){
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}




/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*)){

    hashtable_t* ht;
    ht = malloc(sizeof(hashtable_t));
    DIE(ht == NULL, "Hash Table");

    ht->hmax = hmax;
    ht->compare_function = compare_function;
    ht->hash_function = hash_function;
    ht->size = 0;

    ht->buckets = malloc(hmax * sizeof(struct info *));
    for(int i = 0; i < hmax; i++){
        ht->buckets[i] = ll_create(sizeof(struct info));
    }
    return ht;

}


static ll_node_t *find_key(linked_list_t *bucket, void *key,
	int (*compare_function)(void*, void*), unsigned int *pos){

    ll_node_t * node = bucket->head;
    unsigned  int i = 0;
    while (node != NULL){
        if(compare_function(key, ((struct info*)node->data)->key) == 0){
            *pos = i;
            return  node;
        }
        node = node->next;
        i++;
    }
    return NULL;
}	



/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune tipul ei), in momentul in care
 * se creeaza o noua intrare in hashtable (in cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie
 * a valorii la care pointeaza key si adresa acestei copii trebuie salvata in structura info asociata intrarii din ht.
 * Pentru a sti cati octeti trebuie alocati si copiati, folositi parametrul key_size_bytes.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel put(ht, key_actual, value_actual),
 * valoarea la care pointeaza key_actual poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia unei intrari din hashtable.
 * Nu ne dorim acest lucru, fiindca exista riscul sa ajungem in situatia in care nu mai stim la ce cheie este
 * inregistrata o anumita valoare.
 */
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size){

	// nodul ce trebuie pus in hashtable.
	struct info data;

    unsigned int pos = 0;
	unsigned int idx = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[idx];
	ll_node_t *node = find_key(bucket, key, ht->compare_function, &pos);

	//nodul NU exista in bucket.
	if (node == NULL) {
	
        // nu am gasit cheia in lista
        // alocam data.key si data.value;
        data.value = calloc(1, value_size);
        memcpy(data.value, value, value_size);
        
        data.key = calloc(1, key_size);
        memcpy(data.key, key, key_size);
        ll_add_nth_node(bucket, 0, &data);
        
	}else{
        // am gasit nodul in bucket.
        // am gasit cheia in lista

        free(((struct info*)node->data)->value);
        data.value = calloc(1, value_size);
        memcpy(data.value, value, value_size);
	}
	ht->size ++;
}



void* ht_get(hashtable_t *ht, void *key){


    unsigned int pos = 0;
    unsigned int idx = ht->hash_function(key) % ht->hmax;
    linked_list_t *bucket = ht->buckets[idx];
    ll_node_t *node = find_key(bucket, key, ht->compare_function, &pos);
    if(node != NULL)
        return (((struct info*)node->data)->value);
    return NULL;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable folosind functia put
 * 0, altfel.
 */
int ht_has_key(hashtable_t *ht, void *key){
    unsigned int pos = 0;
    unsigned int idx = ht->hash_function(key) % ht->hmax;
    linked_list_t *bucket = ht->buckets[idx];
    ll_node_t *node = find_key(bucket, key, ht->compare_function, &pos);
    // find_key != NULL
    if( node == NULL )
        return 0;
    return 1;
}


/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o intrare din hashtable (adica memoria
 * pentru copia lui key --vezi observatia de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void ht_remove_entry(hashtable_t *ht, void *key){


	// node = find_key(key, pos)
	// ll_remove_nth(bucket, pos)
	// free(node->data->key)
	// free(node->data->value)
	unsigned int pos = 0;
    unsigned int idx = ht->hash_function(key) % ht->hmax;
    linked_list_t *bucket = ht->buckets[idx];
    ll_node_t *node = find_key(bucket, key, ht->compare_function, &pos);
    if(node != NULL){
        ll_node_t *toBeDeleted = ll_remove_nth_node(ht->buckets[idx],pos);
        free(((struct info*)toBeDeleted->data)->value);
        free(((struct info*)toBeDeleted->data)->key);
        free(toBeDeleted);
    }
    ht->size --;
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable, dupa care elibereaza si memoria folosita
 * pentru a stoca structura hashtable.
 */
void ht_free(hashtable_t *ht){

	// iterati fiecare bucket de mana
    for(int i = 0; i < ht->size; i++){
        while(ht->buckets[i]->size > 0){
            ll_node_t *toBeDeleted = ll_remove_nth_node(ht->buckets[i],0);
            free(((struct info*)toBeDeleted->data)->value);
            free(((struct info*)toBeDeleted->data)->key);
            free(toBeDeleted);
            ht->size --;
        }
    }
}

unsigned int ht_get_size(hashtable_t *ht){
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}

