#include <iostream>
#include <sstream>
#include "../include/Complex.h"

using namespace std;

void printComplexNumInfo(Complex& c) {
    cout << "\tAlgebraic form: " << c << endl;
    cout << "\tamplitude = " << c.amplitude() << ", phase = " << c.phase() << endl;
    cout << "\tIs purely real? " << (c.isReal() ? "Yes" : "No") << endl;
    cout << "\tIs purely imaginary? " << (c.isImaginary() ? "Yes" : "No") << endl;
}

int main() {
    try {
        double r1,r2, i1,i2;
        cout << " Enter First number:" << endl;
        cout << "\tEnter real part: ";
        cin >> r1;
        cout << "\tEnter imaginary part: ";
        cin >> i1;
        Complex a(r1,i1);

        cout << " Enter Second number: " << endl;
        cout << "\tEnter real part: ";
        cin >> r2;
        cout << "\tEnter imaginary part: ";
        cin >> i2;
        Complex b(r2,i2);

        cout << "Entered Numbers info: " << endl;
        cout << endl;
        printComplexNumInfo(a);
        cout << endl;
        printComplexNumInfo(b);

        cout << "A + B = " << (a + b) << endl;
        cout << "A - B = " << (a - b) << endl;
        cout << "A * B = " << (a * b) << endl;

        try {
            cout << "A / B = " << (a / b) << endl;
        } catch (const runtime_error& e) {
            cout << "A / B = ERROR: " << e.what() << endl;
        }
        cout << "A + 2 = " << (a + 2) << endl;
        cout << "2 * B = " << (2 * b) << endl;

        Complex c = a;
        c += b;
        cout << "A += B -> A = " << c << endl;

        cout << "Conjugate of A: " << a.conjugate() << endl;
        cout << "Conjugate of B: " << b.conjugate() << endl;
        cout << "A == B? " << (a == b ? "Yes" : "No") << endl;
        cout << "A != B? " << (a != b ? "Yes" : "No") << endl;

    } catch (exception& e) {
        cout << "An error occurred: " << e.what() << endl;
    }

}