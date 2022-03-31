#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Queue.h"
#define MAX_NODES 10
static int time;



static void print_line_mg(matrix_graph_t* mg){
	printf("\n");
	for(int i = 0; i <= mg->nodes; i++){
		printf("-----");
	}
	printf("\n");	
}

void print_matrix_graph(matrix_graph_t* mg)
{
	printf("\n");
	printf("    |"); 
	for(int i = 0; i < mg->nodes; i++){
		printf("  %d |", i);
	}
	print_line_mg(mg);

	for(int i = 0; i < mg->nodes; i++){
		printf("  %d |", i);
		for(int j = 0; j < mg->nodes; j++){
			printf("  %d |", mg->matrix[i][j]);
		}
		print_line_mg(mg);
	}
}

void print_list_graph(list_graph_t* lg)
{

	for(int i = 0; i < lg->nodes; i++){
		printf("	%d:", i);
		ll_print_int(lg->neighbors[i]);
	}
}

void dfs_list_graph(list_graph_t* lg,
		int node,
		int* visited,  // initial 0 peste tot
		int* t_desc,
		int* t_fin)
{

	visited[node] = 1;
	t_desc[node] = time++;
	printf("%d \n", node);
	
	linked_list_t* neigh = lg_get_neighbours(lg, node);
	for(ll_node_t* curr_node = neigh->head; curr_node != NULL;curr_node = curr_node->next){
		if(!visited[*((int*)curr_node->data)])
			dfs_list_graph(lg, *((int*)curr_node->data), visited, t_desc, t_fin);

	}

	t_fin[node]  = time ++;
}

void dfs_matrix_graph(matrix_graph_t* mg,
		 int node,
		 int* visited,
		 int* t_desc,
		 int* t_fin)
{

	visited[node] = 1;
	t_desc[node] = time++;
	printf("%d \n", node);


	for(int i = 0; i < mg->nodes; i++){
		if(mg->matrix[node][i] && visited[i] == 0){
			dfs_matrix_graph(mg, i, visited, t_desc, t_fin);
		}
	}
	t_fin[node]  = time ++;
}

void bfs_list_graph(list_graph_t* lg, queue_t* q , int node, int* visited, int* t_desc, int* t_fin)
{
	visited[node] = 1;
	q_enqueue(q, &node);

	//algoritmul propriu zis
	while(!q_is_empty(q)){
		int current_node = *((int*)q_front(q));
		q_dequeue(q);
		printf("\n %d", current_node); fflush(stdout);		


		linked_list_t* neigh = lg_get_neighbours(lg,current_node);
		if(neigh){
			for(ll_node_t* it = neigh->head; it != NULL; it = it->next){
				if(visited[(*(int*)it->data)] == 0){ // check if not visited
					q_enqueue(q, it->data);
					visited[(*(int*)it->data)] = 1;
					t_desc[(*(int*)it->data)] = time++;
				}
			}
		}
		t_fin[current_node]  = time ++;
	}

}

void bfs_matrix_graph(matrix_graph_t* mg, queue_t* q , int node, int* visited, int* t_desc, int* t_fin)
{
	visited[node] = 1;
	q_enqueue(q, &node);

	//algoritmul propriu zis
	while(!q_is_empty(q)){
		int current_node = *((int*)q_front(q));
		q_dequeue(q);
		printf("%d ", current_node);		

		for(int i = 0; i < mg->nodes; i++){
			if(mg->matrix[current_node][i] == 1 && visited[i] == 0)
			{
				q_enqueue(q, &i);
				visited[i] = 1;
				t_desc[i] = time++;
			}
		}
	t_fin[node]  = time ++;
	}	
}



int main()
{
	int visited[MAX_NODES] = {0};
	int t_dest[MAX_NODES], t_fin[MAX_NODES];
	list_graph_t* lg = lg_create(MAX_NODES);
	matrix_graph_t* mg = mg_create(MAX_NODES);

	int numbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	printf("-------------------------------- Test ListGraph "
		"--------------------------------\n");
	/* Test add_edge_list_graph has_edge_list_graph */

	lg_add_edge(lg, numbers[0], numbers[1]);
	lg_add_edge(lg, numbers[3], numbers[2]);

	printf("#1:\nOutput: %d\nOutput asteptat: 1\n",
		lg_has_edge(lg, numbers[0], numbers[1]));

	printf("#2:\nOutput: %d\nOutput asteptat: 1\n",
		lg_has_edge(lg, numbers[3], numbers[2]));

	printf("#3:\nOutput: %d\nOutput asteptat: 0\n",
		lg_has_edge(lg, numbers[2], numbers[4]));

	/* Test remove_edge_list_graph */
	lg_remove_edge(lg, numbers[0], numbers[1]);
	printf("#4:\nOutput: %d\nOutput asteptat: 0\n",
		lg_has_edge(lg, numbers[0], numbers[1]));

	/* Test get_neighbours_list_graph */
	lg_add_edge(lg, numbers[0], numbers[1]);
	lg_add_edge(lg, numbers[0], numbers[2]);
	lg_add_edge(lg, numbers[0], numbers[3]);
	lg_add_edge(lg, numbers[0], numbers[4]);

	linked_list_t* l = lg_get_neighbours(lg, numbers[0]);

	printf("#5:\n");
	printf("Output: ");
	ll_print_int(l);
	printf("Output asteptat: 4 3 2 1\n");

	printf("-------------------------------- Test MatrixGraph "
		"--------------------------------\n");
	/* test add edge_matrix_graph has_edge_matrix_graph */
	mg_add_edge(mg, numbers[0], numbers[1]);
	mg_add_edge(mg, numbers[3], numbers[2]);

	printf("#1:\nOutput: %d\nOutput asteptat: 1\n",
		mg_has_edge(mg, numbers[0], numbers[1]));

	printf("#2:\nOutput: %d\nOutput asteptat: 1\n",
		mg_has_edge(mg, numbers[3], numbers[2]));

	printf("#3:\nOutput: %d\nOutput asteptat: 0\n",
		mg_has_edge(mg, numbers[2], numbers[4]));

	/* test remove_edge_matrix_graph */
	mg_remove_edge(mg, numbers[0], numbers[1]);
	printf("#4:\nOutput: %d\nOutput asteptat: 0\n",
		mg_has_edge(mg, numbers[0], numbers[1]));
	



	// printf("-------------------------------- Test BFS DFS "
	// 	"--------------------------------\n");
	// list_graph_t* lg = lg_create(5);
	// matrix_graph_t* mg = mg_create(5);
	// queue_t* q = q_create(sizeof(int), MAX_NODES); 
	// because src -> dest
	// lg_add_edge(lg, numbers[0], numbers[1]);
	// lg_add_edge(lg, numbers[1], numbers[0]);

	// lg_add_edge(lg, numbers[0], numbers[2]);
	// lg_add_edge(lg, numbers[2], numbers[0]);

	// lg_add_edge(lg, numbers[0], numbers[3]);
	// lg_add_edge(lg, numbers[3], numbers[0]);


	// lg_add_edge(lg, numbers[2], numbers[3]);
	// lg_add_edge(lg, numbers[3], numbers[2]);

	// lg_add_edge(lg, numbers[3], numbers[4]);
	// lg_add_edge(lg, numbers[4], numbers[3]);

	// print_list_graph(lg);
	// bfs_list_graph(lg, q, numbers[0], visited, t_dest, t_fin);
	// dfs_list_graph(lg, numbers[0], visited, t_dest, t_fin);
	// q_clear(q);



	// mg_add_edge(mg, numbers[0], numbers[1]);
	// mg_add_edge(mg, numbers[1], numbers[0]);

	// mg_add_edge(mg, numbers[0], numbers[2]);
	// mg_add_edge(mg, numbers[2], numbers[0]);

	// mg_add_edge(mg, numbers[0], numbers[3]);
	// mg_add_edge(mg, numbers[3], numbers[0]);


	// mg_add_edge(mg, numbers[2], numbers[3]);
	// mg_add_edge(mg, numbers[3], numbers[2]);

	// mg_add_edge(mg, numbers[3], numbers[4]);
	// mg_add_edge(mg, numbers[4], numbers[3]);

	// print_matrix_graph(mg);
	// // printf("\nBFS\n");
	// // bfs_matrix_graph(mg, q, numbers[4], visited, t_dest, t_fin);

	// printf("\nDFS\n");
	// dfs_matrix_graph(mg, numbers[4], visited, t_dest, t_fin);

	lg_free(lg);
	mg_free(mg);
	return 0;
}
