#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName* data;		// 이름 배열의 포인터
} tNames;

// 함수 원형 선언 
/*
배열을 이용해서 linked list 구현. 첫 과제와 동일하게
재활용가능
로드네임즈를 변화시켜야 함

*/

// bsearch를 위한 비교 함수
int compare(const void* n1, const void* n2) {
	tName* pn1 = (tName*)n1;
	tName* pn2 = (tName*)n2;

	if (strcmp(pn1->name, pn2->name) > 0)
		return 1;
	else if (strcmp(pn1->name, pn2->name) < 0)
		return -1;
	else if (strcmp(pn1->name, pn2->name) == 0) {	//이름이 같을 때, 성별 비교.
		if (pn1->sex > pn2->sex)
			return 1;
		else if (pn1->sex < pn2->sex)
			return -1;
		else
			return 0;
	}
	else
		return 0;

	/*
		참고
		오름차순으로 정렬을 하고 싶다면 비교함수 내부에서 첫번째 변수가 두번쨰 변수보다 클 때 1을 첫번째 변수가 두번째 변수보다 작을 때 -1을 같을 때 0을 반환하도록 작성하면 됩니다.

	*/
}

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search(const void* key, const void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*)) {
	// nmemb: 배열에서 정렬하고자 하는 원소의 갯수
	// size : 각 원소의 자료형 크기 

	// size_t == unisgned long
	// >> 1 == /2
	// << 1 == *2

	const char* pivot;
	int result;
	int index;

	while (nmemb > 0) {
		index = (nmemb >> 1);
		pivot = base + index * size;
		result = compare(key, pivot);

		if (result == 0)
			return index;

		if (result > 0) {
			base = pivot + size;
			nmemb--;
		}
		nmemb >>= 1;
	}

	return index;

}


// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 구조체 배열은 정렬 리스트(ordered list)이어야 함 이름 들어올때마다 정렬을 유지하는 방식으로. 한칸씩 뒤로 밀어내고 새 이름 저장. 
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용. 정렬을 유지하므로 빨리 찾을 수 있게. 
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 2배씩 증가
void load_names(FILE* fp, int year_index, tNames* names) {

	tName sample;	// 끊어 온 문자열을 저장할 샘플 선언
	char buffer[100];	// 버퍼
	int yc;	// year count를 위한 변수 선언
	int dup = 0;	// 중복 확인용 변수 선언
	int* find;
	int index;

	while (!feof(fp)) {
		char* ptr = fgets(buffer, sizeof(buffer), fp);	// 한 줄 읽어오기	
		if (!ptr) break;

		char* cut = strtok(ptr, ",");	// "," 기준으로 커팅
		sscanf(cut, "%s", sample.name);
		cut = strtok(NULL, ",");

		sscanf(cut, "%c", &sample.sex);
		cut = strtok(NULL, ",");

		sscanf(cut, "%d", &sample.freq[year_index]);

		// 기존에 있는 항목인지 확인 필요, 이제는 정렬되었으므로 binary search 이용
		if (bsearch(sample.name, names->data->name, names->len, sizeof(tName), compare) != NULL) {	// 이름 bsearch 가 NULL 이 아닌 경우. 즉 기존에 있는 이름일 때 
			if ((find = bsearch(&(sample.sex), &names->data->sex, names->len, sizeof(tName), compare)) != NULL) {	// 성별 bsearch 가 NULL 이 아닌 경우. 기존에 있는 이름 + 성별
				names->data[*find].freq[year_index] = sample.freq[year_index]; // 해당 연도 자리에 빈도값만 넣어준다
				dup++;	// 중복인 경우 체크를 위해 dup값 증가
			}
		}


		if (!dup) {	// 만약 중복 없는 항목일 때!!
				if (names->len >= names->capacity) {			// 용량 넘치게 되는지 확인
					names->capacity = names->capacity * 2;	// 용량 두배로
					names->data = (tName*)realloc(names->data, names->capacity * sizeof(tName));	//realloc을 통해 capacity 재할당.

					for (int i = names->len; i < names->capacity; i++) {
						memset(names->data[i].freq, 0, sizeof(names->data[i].freq));	// 재할당한 값 memset을 통해 초기화
					}
				}

			// 이어붙이기 대신 적절한 위치 찾아 밀어내기 
			// binary_search 를 이용해 삽입될 위치 찾아내기
			index = binary_search(sample.name, names->data->name, names->len, sizeof(tName), compare);
			strcpy(names->data[index].name, sample.name);
			names->data[index].sex = sample.sex;
			names->data[index].freq[year_index] = sample.freq[year_index];

			// 실행후 len 값 1 증가
			(names->len)++;

		}
		dup = 0;	// dup 값 초기화
	}

}

// 구조체 배열을 화면에 출력
void print_names(tNames* names, int num_year) {
	// 구조체 배열을 화면에 출력. abc순서로 
	// 구현 필요

	for (int i = 0; i < names->capacity; i++) {
		printf("%s \t %c \t", names->data[i].name, (char)names->data[i].sex);

		for (int j = 0; j < num_year; j++) {	// 배열의 요소 개수만큼 반복 
			printf("%d \t", names->data[i].freq[j]);	 // 배열의 인덱스에 반복문의 변수 i를 지정
		}
		printf("\n");
	}
}



// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames* create_names(void)
{
	tNames* pnames = (tNames*)malloc(sizeof(tNames));

	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName*)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames* pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	tNames* names;

	int num = 0;
	FILE* fp;
	int num_year = 0;

	if (argc == 1) return 0;

	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi(&argv[1][3]);

	for (int i = 1; i < argc; i++)
	{
		num_year++;
		fp = fopen(argv[i], "r");
		assert(fp != NULL);

		int year = atoi(&argv[i][3]); // ex) "yob2009.txt" -> 2009

		fprintf(stderr, "Processing [%s]..\n", argv[i]);

		// 연도별 입력 파일(이름 정보)을 구조체에 저장
		load_names(fp, year - start_year, names);

		fclose(fp);
	}

	// 이름 구조체를 화면에 출력
	print_names(names, num_year);

	// 이름 구조체 해제
	destroy_names(names);

	return 1;
}