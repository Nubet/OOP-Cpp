#pragma once
#include <string>

class Complex {
private:
    double Real;
    double Imag;

public:
    Complex();
    Complex(double value);     // converting constructor
    Complex(double Real, double Imag);

    Complex& operator=(const Complex& c);
    Complex& operator=(double value);

    Complex operator-() const;

    Complex& operator+=(const Complex& c2);
    Complex& operator-=(const Complex& c2);
    Complex& operator*=(const Complex& c2);
    Complex& operator/=(const Complex& c2); // can throw exception

    double amplitude() const;
    double phase() const;
    bool isReal(double epsilon = 1e-10) const;
    bool isImaginary(double epsilon = 1e-10) const;
    Complex conjugate() const;
    std::string toString() const;
    double getReal() const { return Real; }
    double getImag() const { return Imag; }
    void setReal(double r) { Real = r; }
    void setImag(double i) { Imag = i; }


};

Complex operator+(Complex c1, const Complex& c2);
Complex operator-(Complex c1, const Complex& c2);
Complex operator*(Complex c1, const Complex& c2);
Complex operator/(Complex c1, const Complex& c2);

bool operator==(const Complex& c1, const Complex& c2);
bool operator!=(const Complex& c1, const Complex& c2);

std::ostream& operator<<(std::ostream& out, const Complex& c);