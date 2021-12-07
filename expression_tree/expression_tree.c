#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit 연산자 피연산자 구별 
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

typedef struct {
	char stack[MAX_STACK_SIZE];
	int top;
} STACK;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

void createStack(STACK* s) {
	s->top = -1;
}

void push(STACK* s, char item) {
	if (s->top == MAX_STACK_SIZE - 1)	// stack 가득 찬 상태
		return;
	s->stack[++(s->top)] = item;
}

char pop(STACK* s) {
	if (s->top == -1)	// stack 빈 상태
		return;
	return s->stack[(s->top)--];
}

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE* createTree(void) {	// make head. finished
	TREE* tree;
	tree = (TREE*)malloc(sizeof(TREE));
	if (tree)
		tree->root = NULL;
	return tree;
}

/* Deletes all data in tree and recycles memory
*/
void destroyTree(TREE* pTree); // 밑에 있음

static void _destroy(NODE* root) {	// finished
	if (root) {
		_destroy(root->left);
		free(root->data);
		_destroy(root->right);
		free(root);
	}
	return;
}

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE* _makeNode(char ch) {	// finished	//메모리 할당 후 주소 리턴
	NODE *newNode;
	newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		newNode->data = ch;
		newNode->left = NULL;
		newNode->right = NULL;
	}
	return newNode;
}

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree(char* expr, TREE* pTree) {	// finished 
	// 후위표기식을 트리로 변환/ 메인
	STACK s;
	NODE* newNode;
	int i = 0;
	int op1, op2;
	while (expr[i] != NULL){
		if (isdigit(expr[i])) {	// 피연산자(숫자)일 경우
			push(&s, _makeNode(expr[i]));	// 데이터를 노드로 만들어서 스택에 넣어줌 push
		}

		else {	// 연산자일 경우
			newNode = _makeNode(expr[i]);	// 연산자가 들어갈 노드 생성 
			op2 = pop(&s);
			op1 = pop(&s);	// 두 숫자를 pop해서
			newNode->left = op1;
			newNode->right = op2;	// newNode의 두 자식노드로 설정
			push(s, newNode);// 이것을 다시 푸시 
		}
		i++;
	}
	// 트리 모두 만든 후
	if (top) // top 0 이 아닐 때, invalid
		return 0;
	else
		return 1;
}

/* Print node in tree using inorder traversal
*/
void traverseTree(TREE* pTree);	// 아래 구현됨 

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse(NODE* root) {	// finished
	//위에 호출당함. 
	if (root) {
		if (!isdigit(root->data)) {	// 피연산자이면
			printf(root->data);
		}
		else {
			printf("(");
			_traverse(root->left);
			printf(root->data);
			_traverse(root->right);
			printf(")");
		}
	}
}

/* Print tree using inorder right-to-left traversal 
*/
void printTree(TREE* pTree);	// 아래 구현됨 

/* internal traversal function
*/
static void _infix_print(NODE* root, int level) {	// finished
	// 위에서 내부적으로 호출당함. 레벨-내 노드의 깊이에 따라 탭을 하나 입력한 후에 출력하라 등등 
	if (root) {
		_traverse(root->right);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf(root->data);
		printf("\d");
		_traverse(root->left);
	}
}

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix(char* expr) {	
	int op1, op2, value, i = 0;
	int len = strlen(expr);
	char ch;
	STACK s;
	createStack(&s);

	for (i = 0; i < len; i++) {
		ch = expr[i];
		if (isdigit(ch)) {	// 숫자라면
			value = ch - '0';	// 숫자로 바꿔준 후 pop
			push(&s, value);
		}
		else {	// 연산자라면 
			op2 = pop(&s);
			op1 = pop(&s);	// 두 개의 숫자를 pop
			switch (ch) {	// 연산자의 종류에 따라 계산 
				case '+':
					push(&s, op1 + op2);
					break;
				case '-':
					push(&s, op1 - op2);
					break;
				case '*':
					push(&s, op1 * op2);
					break;
				case '/':
					push(&s, op1 / op2);
					break;
			}
		}
		return pop(&s);
	}
}

////////////////////////////////////////////////////////////////////////////////
void destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1) // ctrl d 누르면 종료
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			destroyTree( tree);
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}
