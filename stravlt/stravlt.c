#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE* AVL_Create(void) {
	AVL_TREE* pTree;
	pTree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if (pTree) {
		pTree->root = NULL;
		pTree->count = 0;
	}
	return pTree;
}

/* Deletes all data in tree and recycles memory
*/
static void _destroy(NODE* root);
void AVL_Destroy(AVL_TREE* pTree) {
	if (pTree)
		_destroy(pTree->root);
	free(pTree);
}

static void _destroy(NODE* root) {
	if (root) {
		_destroy(root->left);
		free(root);
		_destroy(root->right);
		free(root);
	}
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
static NODE* _insert(NODE* root, NODE* newPtr);
int AVL_Insert(AVL_TREE* pTree, char* data) {
	NODE* newPtr;

	newPtr = (NODE*)malloc(sizeof(NODE));
	if (!newPtr)
		return 0;

	newPtr->left = NULL;
	newPtr->data = strdup(data);
	newPtr->right = NULL;
	newPtr->height = pTree->root->height;	

	if (pTree->root == 0)
		pTree->root = newPtr;
	else
		_insert(pTree->root, newPtr);
	(pTree->count)++;
	return 1;
}
/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE* rotateRight(NODE* root);
static NODE* rotateLeft(NODE* root);
static NODE* _insert(NODE * root, NODE * newPtr) {
	if (!root) {
		root = newPtr;
		return root;
	}
	
	if (newPtr->data < root->data)
		root->left = _insert(root->left, newPtr);
	else if (newPtr->data > root->data)
		root->right = _insert(root->right, newPtr);
	else 
		return root;

#if BALANCING
	root->height = 1 + max(root->left->height, root->right->height);
	int balance = root->left->height - root->right->height;

	// Left Left Case 
	if (balance > 1 && newPtr->data < root->left->data)
		rotateRight(root);

	// Right Right Case 
	if (balance < -1 && newPtr->data > root->right->data)
		rotateLeft(root);

	// Left Right Case 
	if (balance > 1 && newPtr->data > root->left->data) {
		root->left = rotateLeft(root->left);
		rotateLeft(root);
	}

	// Right Left Case 
	if (balance < -1 && newPtr->data < root->right->data) {
		root->right = rotateRight(root->right);
		rotateLeft(root);
	}
#endif

	return root;
}

static NODE* _makeNode(char* data) {
	NODE* newNode;
	newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		newNode->left = NULL;
		newNode->data = strdup(data);	//strdup으로 고치기
		newNode->right = NULL;
		newNode->height = 1;
	}
	return newNode;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, char* key);
char* AVL_Retrieve(AVL_TREE* pTree, char* key) {
	if (pTree->root)
		_retrieve(pTree->root, key);
	else
		return NULL;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, char* key) {
	if (root) {
		if (key < root->data)
			_retrieve(root->left, key);
		else if (key > root->data)
			_retrieve(root->right, key);
		else
			return root;
	}
	else
		return NULL;
}

/* Prints tree using inorder traversal
*/
static void _traverse(NODE* root);
void AVL_Traverse(AVL_TREE* pTree) {
	_traverse(pTree->root);
}
static void _traverse(NODE* root) {
	if (root) {
		_traverse(root->left);
		printf("%s ", root->data);
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
static void _infix_print(NODE* root, int level);
void printTree(AVL_TREE* pTree) {
	_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
static void _infix_print(NODE* root, int level) {
	if (root) {
		_traverse(root->right);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("%s ", root->data);
		_traverse(root->left);
	}
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE* root) {
	return root->height;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE* rotateRight(NODE* root) { // height 계산. R L 중 높은 height +1
	NODE* tempPtr;

	tempPtr = root->left;
	root->left = tempPtr->right;
	tempPtr->right = root;
	tempPtr->height = max(tempPtr->right->height, tempPtr->left->height);
	return tempPtr;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE* rotateLeft(NODE* root) {
	NODE* tempPtr;

	tempPtr = root->right;
	root->right = tempPtr->left;
	tempPtr->left = root;
	tempPtr->height = max(tempPtr->right->height, tempPtr->left->height);
	return tempPtr;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	 if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	//AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	//printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}

