#include "Matrix.h"
#include "MatrixExceptions.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

Matrix::MatrixData::MatrixData(size_t rows, size_t cols, double initValue) : rows(rows), cols(cols), refCount(1) {
    if (rows == 0 || cols == 0) {
        throw InvalidMatrixDimensionException("Matrix dimensions must be positive");
    }
    data = new double[rows * cols];
    std::fill_n(data, rows * cols, initValue);
}

Matrix::MatrixData::MatrixData(size_t rows, size_t cols, const double* srcData) : rows(rows), cols(cols), refCount(1) {
    if (rows == 0 || cols == 0) {
        throw InvalidMatrixDimensionException("Matrix dimensions must be positive");
    }
    data = new double[rows * cols];
    std::copy_n(srcData, rows * cols, data);
}

Matrix::MatrixData::~MatrixData() { delete[] data; }

void Matrix::swapContents(Matrix& other) { std::swap(sharedData, other.sharedData); }

Matrix::Matrix(size_t rows, size_t cols) {
    if (rows == 0 && cols == 0) {
        sharedData = nullptr;
    }
    else {
        sharedData = new MatrixData(rows, cols, 0.0);
    }
}

Matrix::Matrix(size_t rows, size_t cols, double initValue) { sharedData = new MatrixData(rows, cols, initValue); }

Matrix::Matrix(size_t rows, size_t cols, const double* srcData) { sharedData = new MatrixData(rows, cols, srcData); }

Matrix::Matrix(const Matrix& other) : sharedData(other.sharedData) {
    if (isSharedDataValid()) {
        sharedData->refCount++;
    }
}

Matrix& Matrix::operator=(Matrix other) {
    swapContents(other);
    return *this;
}

Matrix::~Matrix() {
    if (isSharedDataValid()) {
        sharedData->refCount--;
        if (sharedData->refCount == 0) {
            delete sharedData;
        }
    }
}

void Matrix::detachIfNotUniqueOwner() {
    if (isSharedDataValid() && sharedData->refCount > 1) {
        MatrixData* newData = new MatrixData(sharedData->rows, sharedData->cols, sharedData->data);
        sharedData->refCount--;
        sharedData = newData;
    }
}

void Matrix::throwIfDimensionsMismatch(const Matrix& other, const char* operation) const {
    if (!isSharedDataValid() || !other.isSharedDataValid() || sharedData->rows != other.sharedData->rows ||
        sharedData->cols != other.sharedData->cols) {
        throw MatrixDimensionMismatchException(std::string("Matrix dimensions must match for ") + operation);
    }
}

bool Matrix::hasSameDimensionsAs(const Matrix& other) const {
    if (!isSharedDataValid() && !other.isSharedDataValid()) return true;
    if (!isSharedDataValid() || !other.isSharedDataValid()) return false;
    return sharedData->rows == other.sharedData->rows && sharedData->cols == other.sharedData->cols;
}

void Matrix::validateIndex(size_t row, size_t col) const {
    if (!isSharedDataValid() || row >= sharedData->rows || col >= sharedData->cols) {
        throw MatrixIndexOutOfBoundsException("Matrix index out of bounds");
    }
}

size_t Matrix::getIndex(size_t row, size_t col) const {
    validateIndex(row, col);
    return row * sharedData->cols + col;
}

double Matrix::computeDotProduct(size_t row, size_t col, const Matrix& other) const {
    double dotProduct = 0.0;
    for (size_t k = 0; k < sharedData->cols; ++k) {
        dotProduct += read(row, k) * other.read(k, col);
    }
    return dotProduct;
}

double Matrix::read(size_t row, size_t col) const { return sharedData->data[getIndex(row, col)]; }

void Matrix::write(size_t row, size_t col, double value) {
    detachIfNotUniqueOwner();
    sharedData->data[getIndex(row, col)] = value;
}

double Matrix::operator()(size_t row, size_t col) const { return sharedData->data[getIndex(row, col)]; }

Matrix::Mref Matrix::operator()(size_t row, size_t col) { return Mref(*this, row, col); }

Matrix& Matrix::operator+=(const Matrix& other) {
    throwIfDimensionsMismatch(other, "addition");
    detachIfNotUniqueOwner();
    size_t size = sharedData->rows * sharedData->cols;
    for (size_t i = 0; i < size; ++i) {
        sharedData->data[i] += other.sharedData->data[i];
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    throwIfDimensionsMismatch(other, "subtraction");
    detachIfNotUniqueOwner();
    size_t size = sharedData->rows * sharedData->cols;
    for (size_t i = 0; i < size; ++i) {
        sharedData->data[i] -= other.sharedData->data[i];
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (!isSharedDataValid() || !other.isSharedDataValid() || sharedData->cols != other.sharedData->rows) {
        throw MatrixDimensionMismatchException("Matrix dimensions incompatible for multiplication");
    }
    Matrix result(sharedData->rows, other.sharedData->cols);
    for (size_t i = 0; i < sharedData->rows; ++i) {
        for (size_t j = 0; j < other.sharedData->cols; ++j) {
            result.write(i, j, computeDotProduct(i, j, other));
        }
    }
    *this = result;
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (!hasSameDimensionsAs(other)) return false;
    if (sharedData == other.sharedData) return true;
    if (!isSharedDataValid()) return true;

    size_t size = sharedData->rows * sharedData->cols;
    return std::memcmp(sharedData->data, other.sharedData->data, sizeof(double) * size) == 0;
}

bool Matrix::operator!=(const Matrix& other) const { return !(*this == other); }

Matrix operator+(const Matrix& m1, const Matrix& m2) { return Matrix(m1) += m2; }
Matrix operator-(const Matrix& m1, const Matrix& m2) { return Matrix(m1) -= m2; }
Matrix operator*(const Matrix& m1, const Matrix& m2) { return Matrix(m1) *= m2; }

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
    if (!matrix.isSharedDataValid()) return out;
    for (size_t i = 0; i < matrix.sharedData->rows; ++i) {
        for (size_t j = 0; j < matrix.sharedData->cols; ++j) {
            out << matrix(i, j);
            if (j < matrix.sharedData->cols - 1) out << ' ';
        }
        if (i < matrix.sharedData->rows - 1) out << '\n';
    }
    return out;
}

std::istream& operator>>(std::istream& is, Matrix& matrix) {
    if (!matrix.isSharedDataValid()) return is;
    for (size_t i = 0; i < matrix.sharedData->rows; ++i) {
        for (size_t j = 0; j < matrix.sharedData->cols; ++j) {
            double value;
            is >> value;
            matrix.write(i, j, value);
        }
    }
    return is;
}