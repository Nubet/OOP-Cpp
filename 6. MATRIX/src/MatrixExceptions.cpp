#include "MatrixExceptions.h"

MatrixException::MatrixException(const std::string& msg) : message(msg) {}

const char* MatrixException::what() const noexcept { return message.c_str(); }

InvalidMatrixDimensionException::InvalidMatrixDimensionException(const std::string& msg) : MatrixException(msg) {}

MatrixDimensionMismatchException::MatrixDimensionMismatchException(const std::string& msg) : MatrixException(msg) {}

MatrixIndexOutOfBoundsException::MatrixIndexOutOfBoundsException(const std::string& msg) : MatrixException(msg) {}
