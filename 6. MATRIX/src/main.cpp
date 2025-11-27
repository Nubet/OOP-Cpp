#include "Matrix.h"
#include "MatrixExceptions.h"
#include <iostream>

using namespace std;

int main() {
    try {
        Matrix m1(2, 2);
        m1(0, 0) = 1.0;
        m1(0, 1) = 2.0;
        m1(1, 0) = 3.0;
        m1(1, 1) = 4.0;
        cout << "Matrix m1:" << endl;
        cout << m1 << endl << endl;

        cout << "Test 1: Attempting out-of-bounds access..." << endl;
        m1(5, 5);
    }
    catch (const MatrixIndexOutOfBoundsException& e) {
        cout << "Caught MatrixIndexOutOfBoundsException: " << e.what() << endl << endl;
    }

    try {
        cout << "Test 2: Attempting to create a matrix with zero dimensions..." << endl;
        Matrix m2(0, 5, 1.0);
    }
    catch (const InvalidMatrixDimensionException& e) {
        cout << "Caught InvalidMatrixDimensionException: " << e.what() << endl << endl;
    }

    try {
        cout << "Test 3: Attempting to add matrices of different dimensions..." << endl;
        Matrix m3(2, 2, 1.0);
        Matrix m4(3, 3, 2.0);
        Matrix result = m3 + m4;
    }
    catch (const MatrixDimensionMismatchException& e) {
        cout << "Caught MatrixDimensionMismatchException: " << e.what() << endl << endl;
    }

    try {
        cout << "Test 4: Attempting to multiply matrices with incompatible dimensions..." << endl;
        Matrix m5(2, 3, 1.0);
        Matrix m6(2, 2, 2.0);
        Matrix result = m5 * m6;
    }
    catch (const MatrixDimensionMismatchException& e) {
        cout << "Caught MatrixDimensionMismatchException: " << e.what() << endl << endl;
    }
}
