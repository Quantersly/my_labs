#include <stdio.h>

void product(int*, int*);

int main(){
    int i = 0, j = 0;
    product(&i, &j);
    return 0;
}


void product(int *i, int *j) {
    for (int n = 0; n < 10; n++) {
        *i ++;
        (*j) ++;
        printf("%d %d\n", *i, *j);
    }
}
