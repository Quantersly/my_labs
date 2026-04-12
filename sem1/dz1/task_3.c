#include <stdio.h>

int the_sum_of_the_digits(int);

int main() {
	int val = 0, res = 0 ;
	scanf("%d", &val);
	if (val < 0) {
		val *= -1;
	}
	res = the_sum_of_the_digits(val);
	printf("The result is %d\n", res);
	return 0;
}


int the_sum_of_the_digits(int number) {
	int sum = 0;
	while (number != 0) {
		sum += (number % 10);
		number /= 10;
	}
	return sum;
}

