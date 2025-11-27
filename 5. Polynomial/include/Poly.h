#pragma once
#include <iosfwd>
#include <map>

class Poly
{
  public:
    Poly(double value = 0.0);
    Poly(const std::map<int, double>& newTerms);

    Poly operator-() const;

    Poly& operator+=(const Poly& p2);
    Poly& operator-=(const Poly& p2);
    Poly& operator*=(const Poly& p2);

    double operator[](int exponent) const;
    double& operator[](int exponent);
    double operator()(double value) const;
  private:
    std::map<int, double> terms;
    void removeZeros();
    const std::map<int, double>& getTerms() const;
    friend bool operator==(const Poly& p1, const Poly& p2);
    friend std::ostream& operator<<(std::ostream& out, const Poly& p);
};

Poly operator+(Poly p1, const Poly& p2);
Poly operator-(Poly p1, const Poly& p2);
Poly operator*(Poly p1, const Poly& p2);

bool operator!=(const Poly& p1, const Poly& p2);


