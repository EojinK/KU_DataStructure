#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct 
{
	char	*token;	// �̸�. string�� ����Ű�� ������ 
	int		freq;	// ��
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*llink;	
	struct node	*rlink;	
} NODE;

typedef struct
{
	int		count;
	NODE	*pos; // unused
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST* createList(void) {	
	LIST* pList;
	pList = (LIST*)malloc(sizeof(LIST));
	if (pList){
		pList->head = NULL;
		pList->pos = NULL;
		pList->rear = NULL;
		pList->count = 0;
	}
	return pList;
}

/* Deletes all data in list and recycles memory
*/
void destroyList(LIST* pList) {		
	NODE* deletePtr;
	if (pList){
		while (pList->count > 0){
			destroyToken(pList->head->dataPtr);	// ��ū ó��
			free(pList->head->dataPtr);		// ������ �����
			deletePtr = pList->head;		// ��� ó��
			pList->head = pList->head->rlink;
			(pList->count)--;
			free(deletePtr);
		} 
		free(pList);
	} 
	return NULL;
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode(LIST* pList, tTOKEN* dataInPtr) {	
	int dup;
	int success;
	NODE* pPre;
	NODE* pLoc;
	dup = _search(pList, &pPre, &pLoc, dataInPtr);
	if (dup) {		// �ߺ�
		(dataInPtr->freq)++;
		return 2;
	}
	success = _insert(pList, pPre, dataInPtr);
	if (!success)	// �����÷ο�
		return 0;
	return 1;		// ����
}

/* Removes data from list
	return	0 not 
			1 deleted
*/
int removeNode(LIST* pList, char* keyPtr, tTOKEN** dataOut) {	
	int found;
	NODE* pPre;
	NODE* pLoc;
	found = _search(pList, &pPre, &pLoc, keyPtr);
	if (found) 	// ���� ������ ����
		_delete(pList, pPre, pLoc, dataOut);
	return found;
}

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList(LIST* pList, char* pArgu, tTOKEN** pDataOut) {	
	int found;
	NODE* pPre;
	NODE* pLoc;

	found = _search(pList, &pPre, &pLoc, pArgu);
	if (found)
		*pDataOut = pLoc->dataPtr;
	else
		*pDataOut = NULL;
	return found;
}

/* returns number of nodes in list
*/
int countList(LIST* pList) {	
	return (pList->count);
}

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST* pList) {	
	return pList->count ? 0 : 1;
}

int fullList(LIST* pList) {		
	NODE* temp;
	temp = (NODE*)malloc(sizeof(*(pList->rear)));
	if (temp) {
		free(temp);
		return 0;	//not full, return false
	}
	return 1;	//full, return true
}

/* prints data from list (forward)
*/
void printList(LIST* pList) {	
	NODE* temp;
	temp = pList->head;
	while (temp != NULL) {
		printf("%s %d", temp->dataPtr->token, temp->dataPtr->freq);
		temp = temp->rlink;
		if (p != NULL) printf("\n");
	}
}

/* prints data from list (backward)
*/
void printListR(LIST* pList) {		
	NODE* temp;
	temp = pList->rear;
	while (temp != NULL) {
		printf("%s %d", temp->dataPtr->token, temp->dataPtr->freq);
		temp = temp->llink;
		if (p != NULL) printf("\n");
	}
}

/* internal insert function _
	inserts data into a new node
	return	1 if successful
			0 if memory overflow 
*/
static int _insert(LIST* pList, NODE* pPre, tTOKEN* dataInPtr) { // addnode�� �ʿ�
	NODE* pNew;

	if (!(pNew = (NODE*)malloc(sizeof(NODE))))	// �޸� �Ҵ� �� �� ��
		return 0;
	pNew->dataPtr = dataInPtr;
	pNew->rlink = NULL;
	if (pPre == NULL){		// ù ��� ���� ���� ��, Ȥ�� �� ����Ʈ�� ���� �� 
		pNew->rlink = pList->head;
		pList->head = pNew;
		if (pList->count == 0)	// �� ����Ʈ
			pList->rear = pNew;
	}
	else{	// �߰��̳� ���� ���� �� 
		pNew->rlink = pPre->rlink;
		pPre->rlink = pNew;
		pNew->llink = pPre;
		if (pNew->rlink != NULL)
			pNew->rlink->llink = pNew;
	} 
	(pList->count)++;
	(dataInPtr->freq)++;
	return 1;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, tTOKEN** dataOutPtr) {	// removenode�� �ʿ� 
	*dataOutPtr = pLoc->dataPtr;
	if (pPre == NULL)			// ù ��° ��� ���� ��
		pList->head = pLoc->rlink;
	else						// �ٸ� ��� ���� ��
		pPre->rlink = pLoc->rlink;	
	if (pLoc->rlink == NULL)		// �������̶��?
		pList->rear = pPre;
	(pList->count)--;
	free(pLoc->dataPtr->token);
	free(pLoc->dataPtr);
	free(pLoc);
}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search(LIST* pList, NODE** pPre, NODE** pLoc, char* pArgu) {	
	int result;
	*pPre = NULL;
	*pLoc = pList->head;

	if (pList->count == 0)
		return 0;

	if (pArgu > pList->rear->dataPtr) {
		*pPre = pList->rear;
		*pLoc = NULL;
		return 0;
	} 
	while (pArgu >= (*pLoc)->dataPtr) {
		*pPre = *pLoc;
		*pLoc = (*pLoc)->rlink;
		if (pArgu == (*pLoc)->dataPtr) {
			result = 0;
			break;
		}
	} 
	if (result == 0)
		return 1;
	else
		return 0;
}

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN* createToken(char* str) {	// strdup ���.
	tTOKEN* pToken;
	pToken = (tTOKEN*)malloc(sizeof(tTOKEN));
	if (pToken) {
		pToken->token = strdup(str);
		pToken->freq = 0;
	}
	return pToken;
}

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
void destroyToken(tTOKEN* pToken) {		// finished
	pToken->token = NULL;
	pToken->freq = 0;
	free(pToken->token);
	free(pToken);
}

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'F':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	FILE *fp;
	
	//	argc 2�� �ƴϸ� ���� �˷��ְ� ����
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	//	���� ����, ���н� ���� �޽���
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	// �̸� �о���̸鼭 ��ū ����. ��� �̸����� �ϳ����� ��ū�� �������� 
	while(fscanf( fp, "%s", str) == 1)
	{
		pToken = createToken( str);
		
		// insert function call
		ret = addNode( list, pToken);	//strdup(str). createToken�ȿ��� ���
		
		if (ret == 2) // duplicated 
			destroyToken( pToken);
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				printList( list);
				break;
			
			case BACKWARD_PRINT:
				printListR( list);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a string to find: ");
				fscanf( stdin, "%s", str);
				
				if (searchList( list, str, &pToken))
				{
					fprintf( stdout, "(%s, %d)\n", pToken->token, pToken->freq);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a string to delete: ");
				fscanf( stdin, "%s", str);
				
				if (removeNode( list, str, &pToken))
				{
					fprintf( stdout, "%s deleted\n", pToken->token);
					destroyToken( pToken);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
			}
		
		if (action) fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
