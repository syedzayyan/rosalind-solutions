// https://stackoverflow.com/questions/12917727/resizing-an-array-in-c
int getChilds(int n, int m) {
    // Find child population Cn
    if (n < 1) {
        return 0;
    } else {
        if (n == 1) {
            return 1;
        } else {
            int sumChild = 0;
            for (int i = 2; i <= m; i++) {
                sumChild += getChilds(n - i, m);
            }
            return sumChild;
        }
    }
}

int getPop(int n, int m) {
    // Find current population Fn
    return getChilds(n, m) + getChilds(n + 1, m);
}

int main() {
    int n = 6, m = 3; // Example inputs
    printf("Population for n=%d, m=%d: %d\n", n, m, getPop(n, m));
    return 0;
}
