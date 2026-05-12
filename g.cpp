#include<iostream>
using namespace std;

int main() {
    // Matrix 1: 2 Rows, 3 Columns
    int r1 = 2, c1 = 3;
    int mat1[2][3] = { {1, 2, 3}, {4, 5, 6} };

    // Matrix 2: 3 Rows, 2 Columns (r2 must be equal to c1)
    int r2 = 3, c2 = 2;
    int mat2[3][2] = { {1, 2}, {3, 4}, {5, 6} };

    // Result Matrix: r1 x c2 (2x2)
    int res[2][2] = { {0, 0}, {0, 0} };

    // Multiplication Logic
    for(int i = 0; i < r1; i++) {         // Row of first matrix
        for(int j = 0; j < c2; j++) {     // Column of second matrix
            for(int k = 0; k < c1; k++) { // Common dimension
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    // Output dikhane ke liye
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            cout << res[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
