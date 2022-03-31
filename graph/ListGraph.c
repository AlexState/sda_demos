#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"

list_graph_t* lg_create(int nodes)
{

	// neigh[i] = ll_create(sizeof(int))
	list_graph_t* lg = calloc(1, sizeof(list_graph_t));

	lg->nodes = nodes;

	lg->neighbors = calloc(nodes, sizeof(linked_list_t*));
	for(int i = 0; i < nodes; i++){
		lg->neighbors[i] = ll_create(sizeof(int));
	}

	return lg;
}

void lg_add_edge(list_graph_t* graph, int src, int dest)
{

	// src -> dest
	// add_nth(graph->neighbours[src], 0, dest)
	if(graph){
		ll_add_nth_node(graph->neighbors[src],0, &dest);
		
	}
}

static ll_node_t *find_node(linked_list_t *ll, int node, unsigned int* pos)
{
	ll_node_t* current_node = ll->head;
	unsigned int i = 0;
	while (current_node != NULL)
	{	

		if( *(int*)current_node->data == node) {
			*pos = i;
			return current_node;
		}
		current_node = current_node->next;
		i++;

	}

	return NULL;
}

int lg_has_edge(list_graph_t* graph, int src, int dest)
{

	unsigned int pos;
	ll_node_t* find = find_node(graph->neighbors[src], dest, &pos);
	if(find != NULL) return 1;
	return 0;
}

linked_list_t* lg_get_neighbours(list_graph_t* graph, int node)
{

	if(graph){
		return graph->neighbors[node];
	}
	return NULL;
}

void lg_remove_edge(list_graph_t* graph, int src, int dest)
{
	unsigned int pos;
	ll_node_t* find = find_node(graph->neighbors[src], dest, &pos);
	if(find != NULL){
		ll_node_t* toBeDel=  ll_remove_nth_node(graph->neighbors[src], pos);
		free(toBeDel->data);
		free(toBeDel);
	}	

}

void lg_free(list_graph_t* graph)
{
	for(int i = 0; i < graph->nodes; i++){

		while (ll_get_size(graph->neighbors[i]) > 0)
		{
			ll_remove_nth_node(graph->neighbors[i],0);
			
		}
	}
	free(graph->neighbors);
	free(graph);

}
