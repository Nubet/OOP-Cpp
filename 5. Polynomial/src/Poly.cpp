#include "Poly.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>

Poly::Poly(double value) {
    if (value != 0.0) {
        terms[0] = value;
    }
}

Poly::Poly(const std::map<int, double>& terms) : terms(terms) {
    for (const auto& term : terms) {
        if (term.first < 0) {
            throw std::invalid_argument("only non-negative exponents are allowed");
        }
    }
    removeZeros();
}

Poly Poly::operator-() const {
    Poly result;
    for (const auto& term : terms) {
        result.terms[term.first] = -term.second;
    }
    return result;
}

Poly& Poly::operator+=(const Poly& p2) {
    for (const auto& term : p2.terms) {
        this->terms[term.first] += term.second;
    }
    removeZeros();
    return *this;
}

Poly& Poly::operator-=(const Poly& p2) { return *this += (-p2); }

Poly& Poly::operator*=(const Poly& p2) {
    if (this == &p2) {
        Poly copy = *this;
        *this = copy * copy;
        return *this;
    }

    std::map<int, double> multiplicationResult;
    for (const auto& otherTerm : p2.terms) {
        for (const auto& currentTerm : terms) {
            multiplicationResult[otherTerm.first + currentTerm.first] += otherTerm.second * currentTerm.second;
        }
    }
    terms = multiplicationResult;
    removeZeros();
    return *this;
}

const std::map<int, double>& Poly::getTerms() const { return terms; }

double Poly::operator[](int exponent) const {
    auto it = terms.find(exponent);
    return it != terms.end() ? it->second : 0.0;
}

double& Poly::operator[](int exponent) {
    if (exponent < 0) {
        throw std::invalid_argument("only non-negative exponents are allowed");
    }
    return terms[exponent];
}

double Poly::operator()(double value) const {
    double result = 0.0;
    for (const auto& term : terms) {
        result += std::pow(value, term.first) * term.second;
    }
    return result;
}

void Poly::removeZeros() {
    for (auto it = terms.begin(); it != terms.end();) {
        if (it->second == 0.0) {
            it = terms.erase(it);
        }
        else
            ++it;
    }
}

Poly operator+(Poly p1, const Poly& p2) { return p1 += p2; }

Poly operator-(Poly p1, const Poly& p2) { return p1 += (-p2); }

Poly operator*(Poly p1, const Poly& p2) { return p1 *= p2; }

bool operator==(const Poly& p1, const Poly& p2) { return p1.getTerms() == p2.getTerms(); }

bool operator!=(const Poly& p1, const Poly& p2) { return !(p1 == p2); }

std::ostream& operator<<(std::ostream& out, const Poly& p) {
    const auto& terms = p.getTerms();
    if (terms.empty()) return out << "0";

    bool first = true;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        const int exponent = it->first;
        double coefficient = it->second;

        if (coefficient == 0.0) continue;

        // sign
        if (!first)
            out << (coefficient > 0 ? " + " : " - ");
        else if (coefficient < 0)
            out << "-";

        coefficient = std::abs(coefficient);
        first = false;

        // exponent
        if (exponent == 0 || coefficient != 1.0) {
            if (coefficient == static_cast<int>(coefficient))
                out << static_cast<int>(coefficient);
            else
                out << coefficient;
        }

        if (exponent != 0) {
            out << "x";
            if (exponent > 1) out << "^" << exponent;
        }
    }
    return out;
}
