#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$'	// end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtries[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
return	node pointer
NULL if overflow
*/
TRIE* trieCreateNode(void) {
	TRIE* root;
	root = (TRIE*)malloc(sizeof(TRIE));
	if (root) {
		root->index = -1;
	}
	return root;
}

/* Deletes all data in trie and recycles memory
*/
void trieDestroy(TRIE* root) {
	if (root) {
		_destroy(root->left);
		free(root->data);
		_destroy(root->right);
		free(root);
	}
}

/* Inserts new entry into the trie
return	1 success
0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음. index -1이 아닐 때 
int trieInsert(TRIE* root, char* str, int dic_index) {

}

/* Retrieve trie for the requested key
return	index in dictionary (trie) if key found
-1 key not found
*/
int trieSearch(TRIE* root, char* str) {
	int index;

	if (!root)
		return -1;
	if (str < root->subtries[0])
		return trieSearch(root, root->subtries[0]);
	index = root->index- 1;
	while (str < root->subtries[index])
		index--;
	if (str == root->subtries[index])
		return root->subtries[index];

	return trieSearch(root, root->subtries[index]);
}

/* prints all subtries in trie using preorder traversal
*/
void trieList(TRIE* root, char* dic[]) {

}

/* prints all subtries starting with str (as prefix) in trie
ex) "abb" -> "abbess", "abbesses", "abbey", ...
using trieList function
*/
void triePrefixList(TRIE* root, char* str, char* dic[]) {
	int scanCount;
	TRIE* ptr;

	scanCount = 0;
	ptr = root->subtries[0];
	while (scanCount <= root->index) {
		if (ptr)
			_traverse(ptr, process);

		if (scanCount < root->index) {
			process(root->subtries[scanCount].dataPtr);
			ptr = root->subtries[scanCount].rightPtr;
		} 
		scanCount++;
	} 
}

// 아래는 구현하지 않아도 됨
/* makes permuterms for given str
ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
return	number of permuterms
*/
int make_permuterms(char *str, char *permuterms[]);

/* recycles memory for permuterms
*/
void clear_permuterms(char *permuterms[], int size);

/* wildcard search
ex) "ab*", "*ab", "a*b", "*ab*"
using triePrefixList function
*/
void trieSearchWildcard(TRIE *root, char *str, char *dic[]);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie;
	int ret;
	char str[100];
	FILE *fp;
	char *dic[100000];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode();
	
	int index = 0;
	while (fscanf( fp, "%s", str) != EOF)
	{
		ret = trieInsert( trie, str, index);

		if (ret) dic[index++] = strdup( str);
	}
	
	fclose( fp);
	
	fprintf( stdout, "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search
		if (str[strlen(str)-1] == '*')
		{
			str[strlen(str)-1] = 0;
			triePrefixList( trie, str, dic);
		}
		// keyword search
		else
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		
		fprintf( stdout, "\nQuery: ");
	}
	
	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	
	return 0;
}
