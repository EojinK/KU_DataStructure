#include <stdio.h>

int main() {
	int a[400];
	int count = 0;
	int n = 1;

	printf("수열 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 ... 에서 제 400항을 구하여라. \n\n");

	for (int i = 0; i < 400; i++) {
		a[i] = n;
		count++;

		if (count == n) {
			n++;
			count = 0;
		}
	}

	for (int i = 1; i < 401; i++) {
		printf("%d\t", a[i-1]);
		if (i % 10 == 0) printf("\n");
	}

	printf("\n답 : %d. 존나쉽네. \n", a[399]);

	return 0;
}