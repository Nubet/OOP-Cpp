#include <gtest/gtest.h>
#include "../include/Complex.h"
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <string>
#include <iomanip>

static constexpr double EPS = 1e-10;

TEST(Constructors, DefaultConstructorInitializesToZero) {
    Complex c;
    EXPECT_DOUBLE_EQ(c.getReal(), 0.0);
    EXPECT_DOUBLE_EQ(c.getImag(), 0.0);
}

TEST(Constructors, RealOnly) {
    Complex c(3.14);
    EXPECT_DOUBLE_EQ(c.getReal(), 3.14);
    EXPECT_DOUBLE_EQ(c.getImag(), 0.0);
}

TEST(Constructors, RealAndImag) {
    Complex c(2.5, -1.5);
    EXPECT_DOUBLE_EQ(c.getReal(), 2.5);
    EXPECT_DOUBLE_EQ(c.getImag(), -1.5);
}
TEST(Assignment, CopyAssignment) {
    Complex c1(3.0, 4.0);
    Complex c2;
    c2 = c1;
    EXPECT_DOUBLE_EQ(c2.getReal(), 3.0);
    EXPECT_DOUBLE_EQ(c2.getImag(), 4.0);
}

TEST(Assignment, AssignFromComplex) {
    Complex c1(5.0, 6.0);
    Complex c2 = c1;
    EXPECT_DOUBLE_EQ(c2.getReal(), 5.0);
    EXPECT_DOUBLE_EQ(c2.getImag(), 6.0);
}
TEST(Assignment, AssignFromDouble) {
    Complex c;
    c = 7.5;
    EXPECT_DOUBLE_EQ(c.getReal(), 7.5);
    EXPECT_DOUBLE_EQ(c.getImag(), 0.0);
}
TEST(Assignment, SelfAssignment) {
    Complex c(1.0, 2.0);
    c = c;
    EXPECT_NEAR(1.0, c.getReal(), EPS);
    EXPECT_NEAR(2.0, c.getImag(), EPS);
}
TEST(Assignment, FromDouble) {
    Complex c(1.0, 2.0);
    c = 5.0;
    EXPECT_NEAR(5.0, c.getReal(), EPS);
    EXPECT_NEAR(0.0, c.getImag(), EPS);
}
TEST(ComplexUnaryMinus, NegatesRealAndImaginaryParts) {
    Complex c(2.0, -3.0);
    Complex result = -c;
    EXPECT_DOUBLE_EQ(result.getReal(), -2.0);
    EXPECT_DOUBLE_EQ(result.getImag(), 3.0);
}
TEST(ComplexCompoundAddition, AddsTwoComplexNumbers) {
    Complex c1(1.0, 2.0);
    Complex c2(3.0, 4.0);
    c1 += c2;
    EXPECT_DOUBLE_EQ(c1.getReal(), 4.0);
    EXPECT_DOUBLE_EQ(c1.getImag(), 6.0);
}

TEST(ComplexCompoundSubtraction, SubtractsTwoComplexNumbers) {
    Complex c1(5.0, 6.0);
    Complex c2(2.0, 3.0);
    c1 -= c2;
    EXPECT_DOUBLE_EQ(c1.getReal(), 3.0);
    EXPECT_DOUBLE_EQ(c1.getImag(), 3.0);
}
TEST(ComplexCompoundMultiplication, MultipliesTwoComplexNumbers) {
    Complex c1(2.0, 3.0);
    Complex c2(4.0, 5.0);
    c1 *= c2;
    EXPECT_DOUBLE_EQ(c1.getReal(), -7.0);  // (2*4 - 3*5)
    EXPECT_DOUBLE_EQ(c1.getImag(), 22.0);  // (2*5 + 3*4)
}

TEST(ComplexCompoundDivision, DividesTwoComplexNumbers) {
    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);
    c1 /= c2;
    EXPECT_NEAR(c1.getReal(), 2.2, 1e-10);   // (3*1 + 4*2)/5 = 11/5 = 2.2
    EXPECT_NEAR(c1.getImag(), -0.4, 1e-10);  // (4*1 - 3*2)/5 = -2/5 = -0.4
}
TEST(ComplexUtility, AmplitudeCalculation) {
    Complex c(3.0, 4.0);
    EXPECT_DOUBLE_EQ(c.amplitude(), 5.0);  // sqrt(3^2 + 4^2) = 5
}

TEST(ComplexUtility, PhaseCalculation) {
    Complex c(1.0, 1.0);
    EXPECT_DOUBLE_EQ(c.phase(), M_PI/4);  // 45 degrees in radians
}
TEST(ComplexUtility, RealNumberCheck) {
    Complex realNum(5.0, 0.0);
    Complex almostReal(5.0, 1e-11); // very small imaginary part, below typical tolerance
    Complex complexNum(5.0, 1.0);

    //1e-10 is typical tolerance
    //1e-11 used here for edge cases
    //1e-12 very small tolerance
    EXPECT_TRUE(realNum.isReal(1e-10));
    EXPECT_TRUE(almostReal.isReal(1e-10)); // 1e-11 < 1e-10 <- so we assume its real
    EXPECT_FALSE(complexNum.isReal(1e-10)); // 1.0 > 1e-10 <- so not real

    EXPECT_TRUE(almostReal.isReal(1e-9)); // 1e-11 < 1e-9
    EXPECT_FALSE(almostReal.isReal(1e-12)); // 1e-11 > 1e-12
}

TEST(ComplexUtility, ImaginaryNumberCheck) {
    Complex imagNum(0.0, 5.0);
    Complex almostImag(1e-11, 5.0);
    Complex complexNum(1.0, 5.0);

    EXPECT_TRUE(imagNum.isImaginary(1e-10));
    EXPECT_TRUE(almostImag.isImaginary(1e-10));
    EXPECT_FALSE(complexNum.isImaginary(1e-10));

    EXPECT_TRUE(almostImag.isImaginary(1e-9));
    EXPECT_FALSE(almostImag.isImaginary(1e-12));
}

TEST(ComplexUtility, ConjugateOperation) {
    Complex c(3.0, 4.0);
    Complex conj = c.conjugate();
    EXPECT_DOUBLE_EQ(conj.getReal(), 3.0);
    EXPECT_DOUBLE_EQ(conj.getImag(), -4.0);
}
TEST(ComplexComparison, EqualityOperator) {
    Complex c1(2.5, 3.7);
    Complex c2(2.5, 3.7);
    Complex c3(2.5, 3.8);
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
}

TEST(ComplexComparison, InequalityOperator) {
    Complex c1(1.0, 2.0);
    Complex c2(1.0, 2.0);
    Complex c3(1.0, 3.0);
    EXPECT_FALSE(c1 != c2);
    EXPECT_TRUE(c1 != c3);
}
TEST(ComplexStreamOutput, OutputsCorrectFormat) {
    Complex c(2.5, -1.5);
    std::ostringstream output;
    output << c;
    EXPECT_EQ(output.str(), "(2.50,-1.50)");
}

TEST(ComplexUtility, ToStringFormatsCorrectly) {
    Complex c(3.14, -2.71);
    std::string result = c.toString();
    EXPECT_EQ(result, "(3.14,-2.71)");
}

TEST(ComplexErrorHandling, DivisionByZeroThrows) {
    Complex c1(1.0, 1.0);
    Complex c2(0.0, 0.0);
    EXPECT_THROW(c1 / c2, std::runtime_error);
}
TEST(ComplexEdgeCases, AdditionWithZero) {
    Complex c1(2.0, 3.0);
    Complex zero;
    Complex result = c1 + zero;
    EXPECT_DOUBLE_EQ(result.getReal(), 2.0);
    EXPECT_DOUBLE_EQ(result.getImag(), 3.0);
}

TEST(ComplexEdgeCases, MultiplicationByZero) {
    Complex c1(2.0, 3.0);
    Complex zero;
    Complex result = c1 * zero;
    EXPECT_DOUBLE_EQ(result.getReal(), 0.0);
    EXPECT_DOUBLE_EQ(result.getImag(), 0.0);
}

TEST(ComplexEdgeCases, MixedTypesOperations) {
    Complex c(2.0, 3.0);
    Complex result1 = c + 5.0;
    Complex result2 = 5.0 + c;

    EXPECT_DOUBLE_EQ(result1.getReal(), 7.0);
    EXPECT_DOUBLE_EQ(result1.getImag(), 3.0);
    EXPECT_DOUBLE_EQ(result2.getReal(), 7.0);
    EXPECT_DOUBLE_EQ(result2.getImag(), 3.0);
}