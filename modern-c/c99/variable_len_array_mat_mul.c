/*
    Variable‑Length Array (VLA) Matrix Multiplier

    Features: VLAs, for loops.
    Goal: A function void mat_mul(int n, int m, int p, double a[n][m], double b[m][p], double c[n][p])
        that multiplies two matrices.

    Plan:
    * Declare the function with VLA parameters.
    * Implement the triple loop; use static inline and const for clarity.
    * Test with small matrices and verify the result.

    Notes:
        * In `double a[static m]`, the static promises the compiler that this pointer will never be
            NULL and will have at least m number of elements.
        * `inline` suggests the compiler to replace the definition in-place of the call, if possible.
        * `static` in case of complete function inlining, will allow the compiler to delete the standalone static function.
            There's more to static functions, like restricting it to the translation unit but that
            doesn't apply in here.
*/

#include <stdio.h>

static inline void mat_mul(int m, int n, int p, double a[static m][n], double b[static n][p], double c[static m][p]) {
    for (int i = 0; i < m; ++i)
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < p; ++j)
                c[i][j] += a[i][k] * b[k][j];
}

int main() {
    const int m = 2, n = 3, p = 2;
    double a[m][n];
    double b[n][p];
    double c[m][p];

    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3; // {1, 2, 3}
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6; // {4. 5, 6}

    b[0][0] = 1; b[0][1] = 2; // {1, 2}
    b[1][0] = 3; b[1][1] = 4; // {3, 4}
    b[2][0] = 5; b[2][1] = 6; // {5, 6}

    c[0][0] = c[0][1] =
    c[1][0] = c[1][1] = 0;

    mat_mul(m, n, p, a, b, c);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j)
            printf("%f ", c[i][j]);
        printf("\n");
    }

    return 0;
}