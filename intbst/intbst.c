#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node* left;
	struct node* right;
} NODE;

typedef struct
{
	NODE* root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE* BST_Create(void) {
	TREE* pTree;
	pTree = (TREE*)malloc(sizeof(TREE));
	if (pTree)
		pTree->root = NULL;
	return pTree;
}

/* Deletes all data in tree and recycles memory
*/
static void _destroy(NODE* root);

void BST_Destroy(TREE* pTree) {
	if (pTree)
		_destroy(pTree->root);
	free(pTree);
}

/* internal function (not mandatory)
*/
static void _destroy(NODE* root) {
	if (root) {
		_destroy(root->left);
		free(root);
		_destroy(root->right);
		free(root);
	}
	return;
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
static void _insert(NODE* root, NODE* newPtr);

int BST_Insert(TREE* pTree, int data) {
	NODE* newPtr;
	newPtr = (NODE*)malloc(sizeof(NODE));
	if (!newPtr)
		return 0;

	newPtr->left = NULL;
	newPtr->data = data;
	newPtr->right = NULL;

	if (pTree->root == 0)
		pTree->root = newPtr;
	else
		_insert(pTree->root, newPtr);
	return 1;
}

/* internal function (not mandatory)
*/
static void _insert(NODE* root, NODE* newPtr) {	
	if (newPtr->data < root->data)
		_insert(root->left, newPtr);

	else
		_insert(root->right, newPtr);
}

NODE* _makeNode(int data) {
	NODE* newNode;
	newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		newNode->left = NULL;
		newNode->data = data;
		newNode->right = NULL;
	}
	return newNode;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
static NODE* _delete(NODE* root, int dltKey, int* success);
int BST_Empty(TREE* pTree);
int* BST_Retrieve(TREE* pTree, int key);

int BST_Delete(TREE* pTree, int dltKey) {	
	int success;
	NODE* newRoot;

	newRoot = _delete(pTree->root, dltKey, &success);

	if (success) {
		pTree->root = newRoot;
		if (BST_Empty(pTree))	
			pTree->root = NULL;
	}

	return success;
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE* _delete(NODE* root, int dltKey, int* success) {	 // 이중포인터 방지 위해 구조체로 받아줌
	NODE* dltPtr;
	NODE* exchPtr;
	NODE* newRoot;
	void* holdPtr;

	if (!root) {
		*success = 0;
		return NULL;
	}

	if (dltKey < root->data)
		root->left = _delete(root->left, dltKey, success);

	else if (dltKey > root->data)
		root->right = _delete(root->right, dltKey, success);

	else {
		dltPtr = root;
		if (!root->left) {
			free(root);
			newRoot = root->right;
			free(dltPtr);
			*success = 1;
			return newRoot;
		}
		else
			if (!root->right) {
				newRoot = root->left;
				free(dltPtr);
				*success = 1;
				return newRoot;
			}
			else {
				exchPtr = root->left;

				while (exchPtr->right)
					exchPtr = exchPtr->right;

				holdPtr = root;
				root->data = exchPtr->data;
				exchPtr = holdPtr;
				root->left = _delete(root->left, exchPtr->data, success);
			}
	}
	return root;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, int key);
int* BST_Retrieve(TREE* pTree, int key) {	// 반드시 구현할 필요 없음
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
static NODE* _retrieve(NODE* root, int key) {	
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

/* prints tree using inorder traversal
*/
static void _traverse(NODE* root);

void BST_Traverse(TREE* pTree) {	// 오름차순 
	_traverse(pTree->root);
	return;
}

static void _traverse(NODE* root) {
	if (root) {
		_traverse(root->left);
		printf("%d ", root->data);
		_traverse(root->right);
	}
}

/* Print tree using inorder right-to-left traversal
*/
static void _infix_print(NODE* root, int level);

void printTree(TREE* pTree) {	// 트리 90도 회전한 상태로 출력
	_infix_print(pTree->root, 0);
	return;
}

/* internal traversal function
*/
static void _infix_print(NODE* root, int level) {
	if (root) {
		_traverse(root->right);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("%d ", root->data);
		_traverse(root->left);
	}
}

/*
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE* pTree) {
	return (pTree->root == NULL);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	TREE* tree;
	int data;

	// creates a null tree
	tree = BST_Create();

	if (!tree)
	{
		printf("Cannot create a tree!\n");
		return 100;
	}

	fprintf(stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf("%d", &numbers);

	fprintf(stdout, "Inserting: ");

	srand(time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers * 3) + 1; // random number (1 ~ numbers * 3)

		fprintf(stdout, "%d ", data);

		// insert funtion call
		int ret = BST_Insert(tree, data);
		if (!ret) break;
	}
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	BST_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	while (1)
	{
		fprintf(stdout, "Input a number to delete: ");
		int num;
		int ret = scanf("%d", &num);
		if (ret != 1) break;

		ret = BST_Delete(tree, num);
		if (!ret)
		{
			fprintf(stdout, "%d not found\n", num);
			continue;
		}

		// print tree with right-to-left infix traversal
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);

		if (BST_Empty(tree))
		{
			fprintf(stdout, "Empty tree!\n");
			break;
		}
	}

	BST_Destroy(tree);

	return 0;
}


