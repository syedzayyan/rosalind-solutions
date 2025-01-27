#include "../common.h"

long long rabbit_population(int n, int k){
    if (n <= 2){
        return 1;
    }
    return rabbit_population(n-1, k) + k * rabbit_population(n-2,k); //A key observation is that the number of offspring 
    // in any month is equal to the number of rabbits that were alive two months prior
}

int main() {
    int n = 30; // Number of months
    int k = 3; // Rabbit reproduction rate

    // Compute the rabbit population
    long long result = rabbit_population(n, k);

    // Print the result
    printf("%lld\n", result);

    return 0;
}
