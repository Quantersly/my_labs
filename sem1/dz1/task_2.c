#include <stdio.h>

int unwrap(int);
int get_a_palindrome(int);

int main() {
	int val = 0, res = 0;
	scanf("%d", &val);
	if (val >= 0 ) {
		res = get_a_palindrome(val);
		printf("The result is %d\n", res);
	} else {
		printf("The algorithm  id designed for positive numbers only\n");
	}
	return 0;
}


int get_a_palindrome(int x) {
	int unwrapped_x = 0;
	unwrapped_x = unwrap(x);
	while (x != unwrapped_x) {
		x += unwrapped_x;
		unwrapped_x = unwrap(x);
	}
	return x;
}


int unwrap(int y) {
	int unwrapped_y = 0;
	while (y != 0) {
		unwrapped_y *= 10;
		unwrapped_y += (y % 10);
		y /= 10;
	}
	return unwrapped_y;
}

