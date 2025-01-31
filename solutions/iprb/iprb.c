#include "../../common.h"

double iprb(int k, int m, int n){
    double i = (m * m) + (4 * n * n) + (4 * m * n) - (4 * n - m);
    double j = 4 * (k + m + n) * (k + m + n - 1);
    double rst = 1 - i / j;
    return rst;
}


int main(){
    int k = 30; int m = 19; int n = 29;
    double result = iprb(k, m, n);
    printf("%f\n", result);
    return 0;
}