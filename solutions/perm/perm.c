// https://en.m.wikipedia.org/wiki/Heap%27s_algorithm
// https://www.geeksforgeeks.org/heaps-algorithm-for-generating-permutations/


#include "../../common.h"

void print_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void permute(int k, int *array, int size, int *count) {
    if (k == 1) {
        print_array(array, size);
        (*count)++;
        return;
    }
    
    for (int i = 0; i < k - 1; ++i) {
        permute(k - 1, array, size, count);
        swap(&array[k % 2 == 0 ? i : 0], &array[k - 1]);
    }
    permute(k - 1, array, size, count);
}

int main() {
    int n;
    scanf("%d", &n);
    
    if (n < 1 || n > 7) {
        printf("Invalid input. n must be between 1 and 7.\n");
        return 1;
    }
    
    int array[n];
    for (int i = 0; i < n; i++) {
        array[i] = i + 1;
    }
    
    int count = 0;
    permute(n, array, n, &count);
    
    printf("%d\n", count);
    return 0;
}

