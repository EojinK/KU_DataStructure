#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간
#define TOTAL_NUMBER_OF_NAMES	60571	

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
void load_names(FILE* fp, int year_index, tNames* names) {
	/* 
		note
		연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장. tName에. 
		이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
		새로 등장한 이름은 구조체에 추가
		주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
		names->capacity는 2배씩 증가
		이미 있는 이름인지를 먼저 확인하기. 이미 있는 이름이라면 메모리를 다시 할당할 필요 없이 기존 이름에 해당 연도의 빈도값만 초기화. 빈도값을 넣어줌
		없는 이름일 경우 배열 맨 마지막에 추가. capacity를 넘어가는지에 주의하며 realloc
	*/

	tName sample;	// 끊어 온 문자열을 저장할 샘플 선언
	char buffer[100];	// 버퍼
	int yc;	// year count를 위한 변수 선언
	int dup = 0;	// 중복 확인용 변수 선언

	while (!feof(fp)) {
		char* ptr = fgets(buffer, sizeof(buffer), fp);	// 한 줄 읽어오기	
		if (!ptr) break;

		char* cut = strtok(ptr, ",");	// "," 기준으로 커팅
		sscanf(cut, "%s", sample.name);
		cut = strtok(NULL, ",");

		sscanf(cut, "%c", &sample.sex);
		cut = strtok(NULL, ",");

		sscanf(cut, "%d", &sample.freq[year_index]);

		// 기존에 있는 항목인지 확인 필요
		for (yc = 0; yc < names->len; yc++) {	// year count 늘려가면서 확인
			if (strcmp(names->data[yc].name, sample.name) == 0 && names->data[yc].sex == sample.sex) { // 이름, 성별 동시에 기존에 있는가?
				names->data[yc].freq[year_index] = sample.freq[year_index]; // 해당 연도 자리에 빈도값만 넣어준다
				dup++;	// 중복인 경우 체크를 위해 dup값 증가
				break;	// 더 진행할 필요 없으므로 반복 빠져나오기
			}
		}

		if (!dup) {	// 만약 중복 없는 항목일 때!!
			if (names->len >= names->capacity) // 메모리가 부족하지는 않은가?
				if (names->len >= names->capacity) {			// 용량 넘치게 되는지 확인
					names->capacity = names->capacity * 2;	// 용량 두배로
					names->data = (tName*)realloc(names->data, names->capacity * sizeof(tName));	//realloc을 통해 capacity 재할당.

					for (int i = names->len; i < names->capacity; i++) {
						memset(names->data[i].freq, 0, sizeof(names->data[i].freq));	// 재할당한 값 memset을 통해 초기화
					} 
				}

			// 실행후 len 값 1 증가
			(names->len)++;

			// 구조체 data에 붙이기
			strcpy(names->data[yc].name, sample.name);
			names->data[yc].sex = sample.sex;
			names->data[yc].freq[year_index] = sample.freq[year_index];	

		}
		dup = 0;	// dup 값 초기화
	}

}


void print_names(tNames* names, int num_year) {
	// 구조체 배열을 화면에 출력. abc순서로 
	// 구현 필요
	int count = 0;

	for (int i = 0; i < names->capacity; i++) {
		printf("%s \t %c \t", names->data[i].name, (char)names->data[i].sex);

		for (int j = 0; j < num_year; j++) {	// 배열의 요소 개수만큼 반복 
			printf("%d \t", names->data[i].freq[j]);	 // 배열의 인덱스에 반복문의 변수 i를 지정
		}
		printf("\n");
		count++;

		if (count >= TOTAL_NUMBER_OF_NAMES)	break;	
	}
	
}


int compare(const void* n1, const void* n2) {
	// qsort를 위한 비교 함수
	// 구현 필요
	// 이름순으로 정렬
	// 같은 이름에 다른 성별이 있을 경우 이름의 알파벳 순서인 f,m 순서로 정렬하도록.
	// keyword 구조체 qsort

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

	if (argc == 1) return 0;	//argc: main에 전달되는 정보의 개수

	// 이름 구조체 초기화. tNames 초기화 len 0 cap 1
	names = create_names();	//Already Sloved

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi(&argv[1][3]);	//atoi:문자열을 정수로 바꾸는 함수.	//argv: main에 전달되는 문자열의 배열

	for (int i = 1; i < argc; i++)	//파일 하나하나씩 다룸
	{
		num_year++;	// 몇 번째 연도인지. 1, 2, 3, 4 ...
		fp = fopen(argv[i], "r");	//읽기 모드로 파일을 연다
		assert(fp != NULL);	//file open 안됐을때 그냥 종료시킴.

		int year = atoi(&argv[i][3]); // ex) "yob2009.txt" -> 2009. year 추출

		fprintf(stderr, "Processing [%s]..\n", argv[i]);	//진행 상황 사용자에게 알려줌

		// 연도별 입력 파일(이름 정보)을 구조체에 저장
		load_names(fp, year - start_year, names);	//year-start year란, 인덱스 정보. ex.2009년이면 index 0, 10년 index 1. MAX YEAR DURATION에 삽입용

		fclose(fp);
	}

	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort(names->data, names->len, sizeof(tName), compare);

	// 이름 구조체를 화면에 출력
	print_names(names, num_year);

	// 이름 구조체 해제
	destroy_names(names);	//Already Sloved


	return 1;
}


// memory leak 확인해서 에러 나오지 않도록. vaㅣgrind

// 입력 파일 컴마로 구분되어있는데, 이것을 tab으로 바꾼 후 scanf등으로 새 항목 읽어들이기 혹은 한 라인 읽은 후 컴마를 찾아서 토크나이징 하기 등등 
