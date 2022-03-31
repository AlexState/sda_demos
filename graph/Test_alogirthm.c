#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListGraph.h"
#include "Queue.h"
#include "Stack.h"
#include "utils.h"


#define MAX_NODES	100
#define INF		0x3f3f3f3f

/**
 * Adaugati nodurile componentei conexe curente in lista component. 
 */
static void dfs_connected_comps(	list_graph_t *lg, int node,	int *visited, linked_list_t *component) {
	/* TODO */
	visited[node] = 1;
	ll_add_nth_node(component, 0, &node);

	linked_list_t* neigh = lg_get_neighbours(lg, node);
	for(ll_node_t* curr_node = neigh->head; curr_node != NULL;curr_node = curr_node->next){
		if(!visited[*((int*)curr_node->data)])
			dfs_connected_comps(lg,*((int*)curr_node->data), visited, component);

	}
}

static linked_list_t** connected_components(list_graph_t *lg, int *visited, unsigned int *num_comp)
{
	*num_comp = 0;
	linked_list_t **comps = malloc(lg->nodes * sizeof(*comps));
	DIE(!comps, "malloc comps failed");

	/* TODO: adaugati fiecare componenta conexa ca o noua lista in comps */
	for(int i = 0; i < lg->nodes; i++){
		if(!visited[i]){
			comps[*num_comp] = ll_create(sizeof(int)); // int este key pentru grafuri
			(*num_comp) ++;
			dfs_connected_comps(lg, i, visited, comps[(*num_comp) -1]);
		}
	}

	return comps;
}

static void dfs_topo_sort(list_graph_t *lg, int node, int *visited, linked_list_t *sorted)
{
	/* TODO */
	visited[node] = 1;

	linked_list_t* neigh = lg_get_neighbours(lg, node);
	for(ll_node_t* curr_node = neigh->head; curr_node != NULL;curr_node = curr_node->next){
		if(!visited[*((int*)curr_node->data)]){
			dfs_topo_sort(lg, *((int*)curr_node->data), visited, sorted);
		}
	}
	ll_add_nth_node(sorted, 0, &node);
	// add(sorted, 0, node)
}

static linked_list_t* topo_sort(list_graph_t *lg, int *visited)
{
	linked_list_t *sorted = ll_create(sizeof(int));
	
	/* TODO: adaugati nodurile in lista sorted, in ordinea corecta */
	for(size_t i = 0; i < lg->nodes; i++){
		if(!visited[i]){
			dfs_topo_sort(lg,i,visited, sorted);
		}
	}
	return sorted;
}

static void min_path(list_graph_t *lg, int start, int *dist)
{
	/* TODO */
	int visited[MAX_NODES] = {0};

	visited[start] =1;
	dist[start] = 0;
	queue_t* q = q_create(sizeof(int), lg->nodes);
	q_enqueue(q, &start);

	while (!q_is_empty(q)){
		int current = *(int*)q_front(q);
		q_dequeue(q);

		linked_list_t* neigh = lg_get_neighbours(lg, current);
		for(ll_node_t* curr_node = neigh->head; curr_node != NULL;curr_node = curr_node->next){
			if(!visited[*((int*)curr_node->data)]){
				visited[*((int*)curr_node->data)] = 1;
				dist[*((int*)curr_node->data)] = dist[current] + 1;
				q_enqueue(q, curr_node->data);
			}
		}
	}
}

static int check_bipartite(list_graph_t *lg, int *color)
{
	int visited[MAX_NODES] = {0};
	/* TODO: multimile vor avea culorile 1, respectiv 2 */
	int start = 0; // i assume that the first node is 0
	color[start] = 1;
	queue_t* q = q_create(sizeof(int), lg->nodes);
	q_enqueue(q, &start);

	while (!q_is_empty(q)){
		int current = *(int*)q_front(q);
		q_dequeue(q);

		linked_list_t* neigh = lg_get_neighbours(lg, current);
		for(ll_node_t* curr_node = neigh->head; curr_node != NULL;curr_node = curr_node->next){
			if(!visited[*((int*)curr_node->data)]){
				visited[*((int*)curr_node->data)] = 1;
				color[*((int*)curr_node->data)] = (color[current] == 1) ? 2 : 1;
				q_enqueue(q, curr_node->data);
			}
			else if(color[*((int*)curr_node->data)] == color[current]){
				return 0;
			}
		}
	}

	return 1;
}

static void test_connected_comp(void)
{
	unsigned int num_comps, i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t **comps;

	printf("UNDIRECTED graph for the connected components problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	print_list_graph(lg);
	comps = connected_components(lg, visited, &num_comps);

	printf("Found %d connected components:\n", num_comps);
	for (i = 0; i != num_comps; ++i) {
		ll_print_int(comps[i]);
		ll_free(comps + i);
	}
	printf("\n");

	free(comps);
	lg_free(lg);
}

static void test_topo_sort(void)
{
	unsigned int i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t *sorted;

	printf("DIRECTED graph for the topological sort problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
	}

	sorted = topo_sort(lg, visited);

	printf("Topologically sorted nodes:\n");
	ll_print_int(sorted);
	printf("\n");

	ll_free(&sorted);
	lg_free(lg);
}

static void test_min_dist(void)
{
	unsigned int i, nodes, edges;
	int x, y;
	int dist[MAX_NODES] = {0};
	list_graph_t *lg;

	printf("UNDIRECTED graph for the minimum distance problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	for (i = 0; i != nodes; ++i)
		dist[i] = INF;

	min_path(lg, 0, dist);

	printf("Minimum distances to node 0:\n");
	for (i = 0; i != nodes; ++i)
		printf("%u: %d\n", i, dist[i]);
	printf("\n");

	lg_free(lg);
}

static void test_bipartite(void)
{
	unsigned int i, nodes, edges;
	int color[MAX_NODES] = {0};
	int x, y;
	list_graph_t *lg;

	printf("UNDIRECTED graph for the bipartite graph problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}
	
	if (check_bipartite(lg, color)) {
		printf("Nodes with colour 1:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 1)
				printf("%d ", i);
		printf("\nNodes with colour 2:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 2)
				printf("%d ", i);
		printf("\n");
	} else
		printf("The graph is not bipartite\n");

	lg_free(lg);
}

int main(void)
{
	/* Ex 1 */
	// test_connected_comp();

	/* Ex 2 */
	test_topo_sort();

	/* Ex 3 */
	// test_min_dist();

	/* Ex 4 */
	// test_bipartite();

	return 0;
}
