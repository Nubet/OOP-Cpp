#include <gtest/gtest.h>
#include "Matrix.h"
#include "MatrixExceptions.h"
#include <sstream>

TEST(MatrixConstructors, DefaultConstructor) {
    Matrix m(2, 3);
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getColumns(), 3);
    EXPECT_DOUBLE_EQ(m(0, 0), 0.0);
    EXPECT_DOUBLE_EQ(m(1, 2), 0.0);
}

TEST(MatrixConstructors, InitValueConstructor) {
    Matrix m(2, 2, 5.5);
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getColumns(), 2);
    EXPECT_DOUBLE_EQ(m(0, 0), 5.5);
    EXPECT_DOUBLE_EQ(m(1, 1), 5.5);
}

TEST(MatrixConstructors, DataArrayConstructor) {
    double data[] = {1.0, 2.0, 3.0, 4.0};
    Matrix m(2, 2, data);
    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 4.0);
}

TEST(MatrixConstructors, InvalidDimensionsThrowsInitValue) {
    EXPECT_THROW(Matrix(0, 5, 1.0), InvalidMatrixDimensionException);
    EXPECT_THROW(Matrix(5, 0, 1.0), InvalidMatrixDimensionException);
    EXPECT_THROW(Matrix(0, 0, 1.0), InvalidMatrixDimensionException);
}

TEST(MatrixConstructors, InvalidDimensionsThrowsDataArray) {
    double data[] = {1.0, 2.0, 3.0, 4.0};
    EXPECT_THROW(Matrix(0, 4, data), InvalidMatrixDimensionException);
    EXPECT_THROW(Matrix(4, 0, data), InvalidMatrixDimensionException);
    EXPECT_THROW(Matrix(0, 0, data), InvalidMatrixDimensionException);
}

// Test Copy Constructor
TEST(MatrixCopy, CopyConstructor) {
    Matrix m1(2, 2, 3.0);
    Matrix m2(m1);
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getColumns(), 2);
    EXPECT_DOUBLE_EQ(m2(0, 0), 3.0);
    EXPECT_DOUBLE_EQ(m2(1, 1), 3.0);
}

// Test Assignment Operator
TEST(MatrixAssignment, AssignmentOperator) {
    Matrix m1(2, 2, 7.0);
    Matrix m2(3, 3);
    m2 = m1;
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getColumns(), 2);
    EXPECT_DOUBLE_EQ(m2(0, 0), 7.0);
}

TEST(MatrixAssignment, SelfAssignment) {
    Matrix m(2, 2, 4.0);
    m = m;
    EXPECT_DOUBLE_EQ(m(0, 0), 4.0);
}

TEST(MatrixReferenceCount, CopyDoesNotDuplicateData) {
    Matrix m1(2, 2, 1.0);
    Matrix m2 = m1;
    EXPECT_DOUBLE_EQ(m2(0, 0), 1.0);
}

TEST(MatrixReferenceCount, WriteTriggersDetach) {
    Matrix m1(2, 2, 1.0);
    Matrix m2 = m1;
    m2(0, 0) = 5.0;
    EXPECT_DOUBLE_EQ(m2(0, 0), 5.0);
    EXPECT_DOUBLE_EQ(m1(0, 0), 1.0); // m1 should remain unchanged
}

TEST(MatrixAccess, ReadElement) {
    double data[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix m(2, 3, data);
    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(0, 2), 3.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 5.0);
}

TEST(MatrixAccess, WriteElement) {
    Matrix m(2, 2);
    m(0, 0) = 10.0;
    m(1, 1) = 20.0;
    EXPECT_DOUBLE_EQ(m(0, 0), 10.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 20.0);
}

TEST(MatrixAccess, OutOfBoundsThrows) {
    Matrix m(2, 2);
    EXPECT_THROW(m(2, 0), MatrixIndexOutOfBoundsException);
    EXPECT_THROW(m(0, 3), MatrixIndexOutOfBoundsException);
}

TEST(MatrixArithmetic, Addition) {
    double data1[] = {1.0, 2.0, 3.0, 4.0};
    double data2[] = {5.0, 6.0, 7.0, 8.0};
    Matrix m1(2, 2, data1);
    Matrix m2(2, 2, data2);
    Matrix m3 = m1 + m2;
    EXPECT_DOUBLE_EQ(m3(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(m3(0, 1), 8.0);
    EXPECT_DOUBLE_EQ(m3(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(m3(1, 1), 12.0);
}

TEST(MatrixArithmetic, AdditionAssignment) {
    double data1[] = {1.0, 2.0, 3.0, 4.0};
    double data2[] = {5.0, 6.0, 7.0, 8.0};
    Matrix m1(2, 2, data1);
    Matrix m2(2, 2, data2);
    m1 += m2;
    EXPECT_DOUBLE_EQ(m1(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(m1(1, 1), 12.0);
}

TEST(MatrixArithmetic, AdditionDimensionMismatchThrows) {
    Matrix m1(2, 2);
    Matrix m2(3, 3);
    EXPECT_THROW(m1 + m2, MatrixDimensionMismatchException);
}

TEST(MatrixArithmetic, Subtraction) {
    double data1[] = {10.0, 8.0, 6.0, 4.0};
    double data2[] = {1.0, 2.0, 3.0, 4.0};
    Matrix m1(2, 2, data1);
    Matrix m2(2, 2, data2);
    Matrix m3 = m1 - m2;
    EXPECT_DOUBLE_EQ(m3(0, 0), 9.0);
    EXPECT_DOUBLE_EQ(m3(0, 1), 6.0);
    EXPECT_DOUBLE_EQ(m3(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m3(1, 1), 0.0);
}

TEST(MatrixArithmetic, SubtractionAssignment) {
    double data1[] = {10.0, 8.0, 6.0, 4.0};
    double data2[] = {1.0, 2.0, 3.0, 4.0};
    Matrix m1(2, 2, data1);
    Matrix m2(2, 2, data2);
    m1 -= m2;
    EXPECT_DOUBLE_EQ(m1(0, 0), 9.0);
    EXPECT_DOUBLE_EQ(m1(1, 1), 0.0);
}


TEST(MatrixArithmetic, Multiplication) {
    double data1[] = {1.0, 2.0, 3.0, 4.0};
    double data2[] = {5.0, 6.0, 7.0, 8.0};
    Matrix m1(2, 2, data1);
    Matrix m2(2, 2, data2);
    Matrix m3 = m1 * m2;
    // [1 2] * [5 6] = [1*5+2*7  1*6+2*8] = [19 22]
    // [3 4]   [7 8]   [3*5+4*7  3*6+4*8]   [43 50]
    EXPECT_DOUBLE_EQ(m3(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(m3(0, 1), 22.0);
    EXPECT_DOUBLE_EQ(m3(1, 0), 43.0);
    EXPECT_DOUBLE_EQ(m3(1, 1), 50.0);
}

TEST(MatrixArithmetic, MultiplicationNonSquare) {
    double data1[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    double data2[] = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
    Matrix m1(2, 3, data1); // 2x3
    Matrix m2(3, 2, data2); // 3x2
    Matrix m3 = m1 * m2;    // 2x2
    EXPECT_EQ(m3.getRows(), 2);
    EXPECT_EQ(m3.getColumns(), 2);
    // [1 2 3] * [7  8 ] = [1*7+2*9+3*11  1*8+2*10+3*12] = [58  64]
    // [4 5 6]   [9  10]   [4*7+5*9+6*11  4*8+5*10+6*12]   [139 154]
    //           [11 12]
    EXPECT_DOUBLE_EQ(m3(0, 0), 58.0);
    EXPECT_DOUBLE_EQ(m3(0, 1), 64.0);
    EXPECT_DOUBLE_EQ(m3(1, 0), 139.0);
    EXPECT_DOUBLE_EQ(m3(1, 1), 154.0);
}

TEST(MatrixArithmetic, MultiplicationIncompatibleDimensionsThrows) {
    Matrix m1(2, 3);
    Matrix m2(2, 2);
    EXPECT_THROW(m1 * m2, MatrixDimensionMismatchException);
}

TEST(MatrixArithmetic, MultiplicationAssignment) {
    double data1[] = {1.0, 2.0, 3.0, 4.0};
    double data2[] = {5.0, 6.0, 7.0, 8.0};
    Matrix m1(2, 2, data1);
    Matrix m2(2, 2, data2);
    m1 *= m2;
    EXPECT_DOUBLE_EQ(m1(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(m1(1, 1), 50.0);
}


TEST(MatrixComparison, Equality) {
    double data[] = {1.0, 2.0, 3.0, 4.0};
    Matrix m1(2, 2, data);
    Matrix m2(2, 2, data);
    EXPECT_TRUE(m1 == m2);
}

TEST(MatrixComparison, EqualityAfterCopy) {
    Matrix m1(2, 2, 5.0);
    Matrix m2 = m1;
    EXPECT_TRUE(m1 == m2);
}

TEST(MatrixComparison, Inequality) {
    Matrix m1(2, 2, 1.0);
    Matrix m2(2, 2, 2.0);
    EXPECT_TRUE(m1 != m2);
}

TEST(MatrixComparison, DifferentDimensions) {
    Matrix m1(2, 2);
    Matrix m2(3, 3);
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
}

TEST(MatrixStream, InputOperator) {
    Matrix m(2, 2);
    std::istringstream iss("1.5 2.5 3.5 4.5");
    iss >> m;
    EXPECT_DOUBLE_EQ(m(0, 0), 1.5);
    EXPECT_DOUBLE_EQ(m(0, 1), 2.5);
    EXPECT_DOUBLE_EQ(m(1, 0), 3.5);
    EXPECT_DOUBLE_EQ(m(1, 1), 4.5);
}

TEST(MatrixStream, InputOutputRoundtrip) {
    double data[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix m1(2, 3, data);
    std::ostringstream oss;
    oss << m1;

    Matrix m2(2, 3);
    std::istringstream iss(oss.str());
    iss >> m2;

    EXPECT_TRUE(m1 == m2);
}
TEST(MatrixMref, MrefConversionToDouble) {
    Matrix m(2, 2, 5.0);
    m(0, 0) = 42.0;
    double value = m(0, 0);
    EXPECT_DOUBLE_EQ(value, 42.0);
}

TEST(MatrixMref, MrefReadDoesNotTriggerDetach) {
    Matrix m1(2, 2, 1.0);
    Matrix m2 = m1;

    double val1 = m2(0, 0);
    double val2 = m2(1, 1);
    EXPECT_DOUBLE_EQ(val1, 1.0);
    EXPECT_DOUBLE_EQ(val2, 1.0);


    EXPECT_DOUBLE_EQ(m1(0, 0), 1.0);
}

TEST(MatrixMref, MrefWriteTriggersDetach) {
    Matrix m1(2, 2, 1.0);
    Matrix m2 = m1;
    m2(0, 0) = 99.0;
    EXPECT_DOUBLE_EQ(m2(0, 0), 99.0);
    EXPECT_DOUBLE_EQ(m1(0, 0), 1.0);
}
