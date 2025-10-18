#include "../include/Complex.h"
#include <cmath>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <sstream>

Complex::Complex() {
    Real = 0.0;
    Imag = 0.0;
}

Complex::Complex(double value) {
    Real = value;
    Imag = 0.0;
}

Complex::Complex(double Real, double Imag) {
    this->Real = Real;
    this->Imag = Imag;
}

Complex& Complex::operator=(const Complex& c) {
    Real = c.Real;
    Imag = c.Imag;
    return *this;
}

Complex& Complex::operator=(double value) {
    Real = value;
    Imag = 0.0;
    return *this;
}

Complex Complex::operator-() const {
    return Complex(-Real, -Imag);
}

Complex& Complex::operator+=(const Complex& c2) {
    Real += c2.Real;
    Imag += c2.Imag;
    return *this;
}

Complex& Complex::operator-=(const Complex& c2) {
    Real -= c2.Real;
    Imag -= c2.Imag;
    return *this;
}

Complex& Complex::operator*=(const Complex& c2) {
    double newReal = Real * c2.Real - Imag * c2.Imag;
    double newImag = Real * c2.Imag + Imag * c2.Real;
    Real = newReal;
    Imag = newImag;
    return *this;
}

Complex& Complex::operator/=(const Complex& c2) {
    double denom = c2.Real * c2.Real + c2.Imag * c2.Imag;
    if (denom == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    double newReal = (Real * c2.Real + Imag * c2.Imag) / denom;
    double newImag = (Imag * c2.Real - Real * c2.Imag) / denom;
    Real = newReal;
    Imag = newImag;
    return *this;
}

//  Utility functions
double Complex::amplitude() const {
    return std::hypot(Real, Imag);
}

double Complex::phase() const {
    return std::atan2(Imag, Real);
}
bool Complex::isReal(double epsilon) const {
    return std::fabs(Imag) < epsilon;
}

bool Complex::isImaginary(double epsilon) const {
    return std::fabs(Real) < epsilon;
}
Complex Complex::conjugate() const {
    return Complex(Real, -Imag);
}

std::string Complex::toString() const {
    std::ostringstream output;
    output << std::fixed << std::setprecision(2);
    output << "(" << Real << "," << Imag << ")";
    return output.str();
}
// non member 
Complex operator+(Complex c1, const Complex& c2) { return c1 += c2; }
Complex operator-(Complex c1, const Complex& c2) { return c1 -= c2; }
Complex operator*(Complex c1, const Complex& c2) { return c1 *= c2; }
Complex operator/(Complex c1, const Complex& c2) {return c1 /= c2;}


static inline bool isNearEq(double a, double b, double eps = 1e-10) {
    return std::fabs(a - b) < eps;
}

bool operator==(const Complex& c1, const Complex& c2) {
    return  isNearEq(c1.getReal(), c2.getReal()) && isNearEq(c1.getImag(), c2.getImag());
}

bool operator!=(const Complex& c1, const Complex& c2) { return !(c1 == c2); }

std::ostream& operator<<(std::ostream& out, const Complex& c) {
    return out << c.toString();
}
