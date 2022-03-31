/**
 * SD, 2021
 * 
 * Lab #9, BST & Heap
 * 
 * Task #1 - Binary Search Tree implementation
 */

#include "bst.h"
#include "utils.h"

/**
 * Helper function to create a node
 * @data: the data to be added in the node
 * @data_size: data's size
 */
static bst_node_t *__bst_node_create(void *data, size_t data_size)
{
    bst_node_t *bst_node;  

    bst_node = malloc(sizeof(*bst_node));

    DIE(bst_node == NULL, "bst_node malloc");

    bst_node->left = bst_node->right = NULL;

    bst_node->data = malloc(data_size);
    DIE(bst_node->data == NULL, "bst_node->data malloc");
    memcpy(bst_node->data, data, data_size);

    return bst_node;
}

bst_tree_t *bst_tree_create(size_t data_size,
                            int (*cmp_f)(const void *, const void *))
{
    bst_tree_t *bst_tree;

    bst_tree = malloc(sizeof(*bst_tree));
    DIE(bst_tree == NULL, "bst_tree malloc");

    bst_tree->root  = NULL;
    bst_tree->data_size = data_size;
    bst_tree->cmp   = cmp_f;

    return bst_tree;
}

void bst_tree_insert(bst_tree_t *bst_tree, void *data)
{

    if (!bst_tree)
        return;    
    
    int rc;
    bst_node_t *current_node    = bst_tree->root;
    bst_node_t *node    = __bst_node_create(data, bst_tree->data_size);


    if( bst_tree->root == NULL){
        bst_tree->root = node;
        return;
    }

    while(current_node != NULL){ // cam redundat
        rc = bst_tree->cmp(data, current_node->data);
        if(rc < 0){
            // left
            if(current_node->left == NULL){
                current_node->left = node;
                return;
            }
            current_node = current_node->left;    
        }
        else if(rc > 0){
            // right
                if(current_node->right == NULL){
                    current_node->right = node;
                    return;
                }
 
            current_node = current_node->right;    
        }else{
            // the node is already inserted in tree
            return;
        }
    }
}



/**
 * Helper function to remove an element from a BST
 * @bst_node: the binary search subtree's root where to remove the element from
 * @data: the data that is contained by the node which has to be removed
 * @data_size: data size
 * @cmp: function used to compare the data contained by two nodes
 */
static bst_node_t *__bst_tree_remove(bst_node_t *bst_node,
                                    void *data, size_t data_size,
                                    int (*cmp)(const void *, const void *))
{
    int rc;

    if (!bst_node)
        return NULL;

    rc = cmp(data, bst_node->data);

    if (rc < 0) {
        // data < node->data
        // node->left = remove(left)
        bst_node->left = __bst_tree_remove(bst_node->left, bst_node->data, data_size, cmp);
    } else if (rc > 0) {

        bst_node->right = __bst_tree_remove(bst_node->right, bst_node->data, data_size, cmp);        
    } else {
       int rc_left;
       int rc_right;
       
       bst_node_t* max_left = bst_node->left;
       bst_node_t* min_right = bst_node->right;

       bst_node_t* parent_l = bst_node;
       bst_node_t* parent_r = bst_node;

       // nodul e frunza
       if(max_left == NULL && min_right == NULL){
           free(bst_node->data);
           free(bst_node);
           return NULL;
       }
       else if(max_left == NULL )
       {
            while(min_right->right != NULL){
                parent_r = min_right;
                min_right  = min_right->left;
            }

            //update parrent
            parent_r->right = min_right->right;
            
            // update the node that will be switched
            min_right->left = bst_node->left;
            min_right->right= bst_node->right;
            
            free(bst_node->data);
            free(bst_node);

            return min_right;
       }else if(min_right == NULL){

            while(max_left->right != NULL){
                parent_l = max_left;
                max_left  = max_left->right; 
            }
            //update parrent
            parent_l->right = max_left->left;
            // update the node that will be switched
            max_left->left = bst_node->left;
            max_left->right= bst_node->right;

            free(bst_node->data);
            free(bst_node);
            return max_left;
       }

        // avem ambii copii liberi.
       while(max_left->right != NULL){
            parent_l = max_left;
            max_left  = max_left->right; 
       }

       while(min_right->right != NULL){
           parent_r = min_right;
           min_right  = min_right->left;
       }

        rc_left = cmp(bst_node->data, max_left->data);
        rc_right = cmp(min_right->data, bst_node->data);          

        if(rc_left < rc_right){
            parent_l->right = max_left->left;

            max_left->left = bst_node->left;
            max_left->right= bst_node->right;
            return max_left;
        }
        else{
            parent_r->right = min_right->right;

            min_right->left = bst_node->left;
            min_right->right= bst_node->right;
            return min_right;
        }
    }

}

void bst_tree_remove(bst_tree_t *bst_tree, void *data)
{
    bst_tree->root = __bst_tree_remove(bst_tree->root, data,
                                       bst_tree->data_size, bst_tree->cmp);
}

/**
 * Free the left and the right subtree of a node, its data and itself
 * @b_node: the node which has to free its children and itself
 * @free_data: function used to free the data contained by a node
 */
static void __bst_tree_free(bst_node_t *bst_node, void (*free_data)(void *))
{
    if (!bst_node)
        return;

    /* TODO */
    __bst_tree_free(bst_node->left, free_data);
    __bst_tree_free(bst_node->right, free_data);
    free_data(bst_node->data);
    free_data(bst_node);
}

void bst_tree_free(bst_tree_t *bst_tree, void (*free_data)(void *))
{
    __bst_tree_free(bst_tree->root, free_data);
    free(bst_tree);
}

static void
__bst_tree_print_inorder(bst_node_t* bst_node, void (*print_data)(void*))
{
	if (!bst_node)
		return;
        
    __bst_tree_print_inorder(bst_node->left, print_data);
    print_data(bst_node->data);
    __bst_tree_print_inorder(bst_node->right, print_data);
}

void
bst_tree_print_inorder(bst_tree_t* bst_tree, void (*print_data)(void*))
{
	__bst_tree_print_inorder(bst_tree->root, print_data);
}
