#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

struct avl_node_s {
	struct avl_node_s *left;
	struct avl_node_s *right;
	int value;
	int isroot;
};

typedef struct avl_node_s avl_node_t;

struct avl_tree_s {
	struct avl_node_s *root;
	int size;
};

typedef struct avl_tree_s avl_tree_t;


/* Create a new AVL tree. */
avl_tree_t *avl_create() {
	avl_tree_t *tree = NULL;

	if( ( tree = malloc( sizeof( avl_tree_t ) ) ) == NULL ) {
		return NULL;
	}

	tree->root = NULL;
	tree->size = 0;

	return tree;
}

/* Initialize a new node. */
avl_node_t *avl_create_node() {
	avl_node_t *node = NULL;
	
	if( ( node = malloc( sizeof( avl_node_t ) ) ) == NULL ) {
		return NULL;
	}

	node->left = NULL;
	node->right = NULL;
	node->value = 0;
	node->isroot = 0;
	return node;	
}

/* Find the height of an AVL node recursively */
int avl_node_height( avl_node_t *node ) {
	int height_left = 0;
	int height_right = 0;

	if( node->left ) height_left = avl_node_height( node->left );
	if( node->right ) height_right = avl_node_height( node->right );

	return height_right > height_left ? ++height_right : ++height_left;
}

/* Find the balance of an AVL node */
int avl_balance_factor( avl_node_t *node ) {
	int bf = 0;

	if( node->left  ) bf += avl_node_height( node->left );
	if( node->right ) bf -= avl_node_height( node->right );

	return bf ;
}

/* Left Left Rotate */
avl_node_t *avl_rotate_leftleft( avl_node_t *node ) {
 	avl_node_t *a = node;
	avl_node_t *b = a->left;
	
	a->left = b->right;
	b->right = a;

	return( b );
}

/* Left Right Rotate */
avl_node_t *avl_rotate_leftright( avl_node_t *node ) {
	avl_node_t *a = node;
	avl_node_t *b = a->left;
	avl_node_t *c = b->right;
	
	a->left = c->right;
	b->right = c->left; 
	c->left = b;
	c->right = a;

	return( c );
}

/* Right Left Rotate */
avl_node_t *avl_rotate_rightleft( avl_node_t *node ) {
	avl_node_t *a = node;
	avl_node_t *b = a->right;
	avl_node_t *c = b->left;
	
	a->right = c->left;
	b->left = c->right; 
	c->right = b;
	c->left = a;

	return( c );
}

/* Right Right Rotate */
avl_node_t *avl_rotate_rightright( avl_node_t *node ) {
	avl_node_t *a = node;
	avl_node_t *b = a->right;
	
	a->right = b->left;
	b->left = a; 

	return( b );
}

/* Balance a given node */
avl_node_t *avl_balance_node( avl_node_t *node ) {
	avl_node_t *newroot = NULL;

	/* Balance our children, if they exist. */
	if( node->left )
		node->left  = avl_balance_node( node->left  );
	if( node->right ) 
		node->right = avl_balance_node( node->right );

	int bf = avl_balance_factor( node );

	if( bf >= 2 ) {
		/* Left Heavy */	

		if( avl_balance_factor( node->left ) <= -1 ) 
			newroot = avl_rotate_leftright( node );
		else 
			newroot = avl_rotate_leftleft( node );

	} else if( bf <= -2 ) {
		/* Right Heavy */

		if( avl_balance_factor( node->right ) >= 1 )
			newroot = avl_rotate_rightleft( node );
		else 
			newroot = avl_rotate_rightright( node );

	} else {
		/* This node is balanced -- no change. */

		newroot = node;
	}

	return( newroot );	
}

/* Balance a given tree */
void avl_balance( avl_tree_t *tree ) {

	avl_node_t *newroot = NULL;

	newroot = avl_balance_node( tree->root );

	if( newroot != tree->root )  {
		tree->root = newroot; 
	}
}

/* Insert a new node. */
void avl_insert( avl_tree_t *tree, int value, int basic_insert, int k) {
	avl_node_t *node = NULL;
	avl_node_t *next = NULL;
	avl_node_t *last = NULL;

	/* Well, there must be a first case */ 	
	if( tree->root == NULL ) {
		node = avl_create_node();
		node->value = value;
		node->isroot = 1;
		tree->root = node;
		tree->size = 0;

	/* Okay.  We have a root already.  Where do we put this? */
	// if this is a constructor build, then just insert the first
	// twenty indexs. Else, don't
	} else {
		int compared;
		next = tree->root;
		while( next != NULL ) {
			last = next;
			compared = COMPARE(value, next->value);
			if (compared == 1) {
				next = next->left;
			} else if (compared == 2) {
				next = next->right;
			} else {
				printf("There was an error");
			}
		}

		node = avl_create_node();
		node->value = value;
		//int compared = COMPARE(value, last->value);
		if (compared == 1) {
			last->left = node;
		} else if (compared == 2) {
			last->right = node;
		} else {
			printf("There was an error(2)");
		}
		
	}
	tree->size++;
	if (tree->size > k)
	{
		avl_node_t *smallest_node = tree->root;
		avl_node_t *prev = smallest_node;
		while (smallest_node->right != NULL )
		{
			prev = smallest_node;
			smallest_node = smallest_node->right;
		}
		prev->right = smallest_node->left;
		free(smallest_node);
		tree->size--;
	}
	avl_balance( tree );
}

int transverse(avl_node_t *n, int i, int *Best) {
	if (n->left != NULL) {
		i = transverse(n->left, i, Best);
	} 
	Best[i] = n->value;
	i++;

	if (n->right != NULL) {
		i = transverse(n->right, i, Best);
	}

	return i;
}

void clean_tree(avl_node_t *n)
{
	if (n->left != NULL)
		clean_tree(n->left);
	if (n->right != NULL)
		clean_tree(n->right);
	free(n);
	n = NULL;
}

int doalg(int n, int k, int *Best) {

	static int seen = 0;//Delete me later
	if (seen == 0)
	{
		printf("AVL\n");
		seen = 1;
	}


	// construct the avl tree of N elements.
	avl_tree_t *tree = avl_create();

	// build avl tree of size k
	int i;
	for (i = 1; i < n + 1; i++) {
		avl_insert(tree, i, 0, k);
	}

	transverse(tree->root, 0, Best);
	
	//for (int i = 0; i < k; i++) {
	//	printf("%d,", Best[i]);
	//}
	//printf("\n\n");
	clean_tree(tree->root);
	free(tree);
	return 1;
}
