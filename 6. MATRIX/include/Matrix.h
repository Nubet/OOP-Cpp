#pragma once
#include <ostream>
#include <istream>
#include <cstddef>

class Matrix {
public:
    class Mref;

    explicit Matrix(size_t rows = 0, size_t cols = 0);
    explicit Matrix(size_t rows, size_t cols, double initValue);
    Matrix(size_t rows, size_t cols, const double* data);
    Matrix(const Matrix& other);

    Matrix& operator=(Matrix other);
    ~Matrix();

    size_t getRows() const { return sharedData ? sharedData->rows : 0; }
    size_t getColumns() const { return sharedData ? sharedData->cols : 0; }

    double operator()(size_t row, size_t col) const;
    Mref operator()(size_t row, size_t col);

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);

private:
    void swapContents(Matrix& other);
    void detachIfNotUniqueOwner();
    size_t getIndex(size_t row, size_t col) const;
    bool isSharedDataValid() const { return sharedData != nullptr; }
    void throwIfDimensionsMismatch(const Matrix& other, const char* operation) const;
    bool hasSameDimensionsAs(const Matrix& other) const;
    double computeDotProduct(size_t row, size_t col, const Matrix& other) const;
    double read(size_t row, size_t col) const;
    void write(size_t row, size_t col, double value);
    void validateIndex(size_t row, size_t col) const;

    struct MatrixData {
        size_t rows;
        size_t cols;
        double* data;
        size_t refCount;

        MatrixData(size_t rows, size_t cols, double initValue);
        MatrixData(size_t rows, size_t cols, const double* srcData);
        ~MatrixData();
    };

    MatrixData* sharedData;

};

class Matrix::Mref {
    friend class Matrix;
    Matrix& m;
    size_t row, col;
    Mref(Matrix& matrix, size_t r, size_t c) : m(matrix), row(r), col(c) {
        m.validateIndex(r, c);
    }

public:
    operator double() const {
        return m.read(row, col);
    }

    Matrix::Mref& operator=(double value) {
        m.write(row, col, value);
        return *this;
    }

};

Matrix operator+(const Matrix& m1, const Matrix& m2);
Matrix operator-(const Matrix& m1, const Matrix& m2);
Matrix operator*(const Matrix& m1, const Matrix& m2);
