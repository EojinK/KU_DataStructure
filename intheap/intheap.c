#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP* heapCreate(int capacity) {
	HEAP* heap;
	heap = (HEAP*)malloc(sizeof(HEAP));
	if (!heap)
		return NULL;
	heap->heapArr = (int*)malloc(capacity, sizeof(int*));
	heap->last = -1;
	heap->capacity = capacity;
	return heap;
}

/* Free memory for heap
*/
void heapDestroy(HEAP* heap) {
	if (heap) {
		while (heap->last != -1) {
			free(heap->heapArr[heap->last]);
			(heap->last)--;
		}
		free(heap);
	}
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP* heap, int data) {
	if (heap->last == -1) {	// empty heap
		heap->last = 0;
		heap->heapArr[heap->last] = data;
		return 1;
	} 

	if (heap->last == heap->capacity - 1)	// full heap
		return 0;

	++(heap->last);
	++(heap->capacity);

	heap->heapArr[heap->last] = data;
	_reheapUp(heap, heap->last);
	return 1;
}

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP* heap, int index) {
	int parent;
	void** heapArr;
	void* hold;

	if (index) {
		heapArr = heap->heapArr;
		parent = (index - 1) / 2;
		if (heapArr[index] > heapArr[parent]) {	// child is greater than its parent
			hold = heapArr[parent];
			heapArr[parent] = heapArr[index];
			heapArr[index] = hold;
			_reheapUp(heap, parent);
		} 
	} 
}

/* Deletes index of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP* heap, int* data) {
	if (heap->capacity == 0)	// empty heap
		return 0;

	*data = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[heap->last];
	(heap->last)--;
	(heap->capacity)--;
	_reheapDown(heap, 0);
	return 1;
}

/* Reestablishes heap by moving data in index down to its correct location in the heap
*/
static void _reheapDown(HEAP* heap, int index) {	// index 0 
	void* hold;
	void* leftData;
	void* rightData;
	int largeLoc;
	int last;

	last = heap->last;

	if ((index * 2 + 1) <= last) {
		leftData = heap->heapArr[index * 2 + 1];
		if ((index * 2 + 2) <= last) 
			rightData = heap->heapArr[index * 2 + 2];
		else
			rightData = NULL;

		if ((!rightData) || (leftData > rightData))
			largeLoc = index * 2 + 1;

		else
			largeLoc = index * 2 + 2;

		if (heap->heapArr[index] < heap->heapArr[largeLoc]) {
			hold = heap->heapArr[index];
			heap->heapArr[index] = heap->heapArr[largeLoc];
			heap->heapArr[largeLoc] = hold;
			_reheapDown(heap, largeLoc);
		} 
	} 
}

/* Print heap array */
void heapPrint(HEAP* heap) {
	for (int i = 0; i < heap->capacity; i++)
		printf("%6d", heap->heapArr);
}

int main(void)
{
	HEAP *heap;
	int data;
	int i;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		printf( "Deleting %d: ", data);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}

