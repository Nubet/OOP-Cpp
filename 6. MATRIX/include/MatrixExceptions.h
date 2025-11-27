#pragma once
#include <exception>
#include <string>

class MatrixException : public std::exception {
protected:
    std::string message;

public:
    explicit MatrixException(const std::string& msg);
    const char* what() const noexcept override;
};

class InvalidMatrixDimensionException : public MatrixException {
public:
    explicit InvalidMatrixDimensionException(const std::string& msg);
};

class MatrixDimensionMismatchException : public MatrixException {
public:
    explicit MatrixDimensionMismatchException(const std::string& msg);
};

class MatrixIndexOutOfBoundsException : public MatrixException {
public:
    explicit MatrixIndexOutOfBoundsException(const std::string& msg);
};

