#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <format>
#include <random>
#include <stdexcept>
#include <complex>
#include <mpi.h>

using namespace std;

template <typename T>

class Matrix {
private:
    T* _Matrixptr;
    size_t _lines;
    size_t _columns;
    inline static const double epsilon = 0.001;

public:
    Matrix() :_Matrixptr(nullptr), _lines(0), _columns(0) {}

    Matrix(size_t lines, size_t columns, T value) : _lines(lines), _columns(columns) {
        _Matrixptr = new T[lines * columns];
        for (size_t i = 0; i < lines * columns; i++) {
            _Matrixptr[i] = value;
        }
    }

    Matrix(size_t lines, size_t columns, int inf, int sup) : _lines(lines), _columns(columns) {
        _Matrixptr = new T[lines * columns];
        std::random_device engine;
        std::uniform_int_distribution distribution(std::min(inf, sup), std::max(inf, sup));
        for (size_t i = 0; i < columns * lines; i++)
        {
            _Matrixptr[i] = distribution(engine);
        }
    }

    Matrix(size_t lines, size_t columns, float inf, float sup) : _lines(lines), _columns(columns) {
        _Matrixptr = new T[lines * columns];
        std::random_device engine;
        std::uniform_real_distribution distribution(std::min(inf, sup), std::max(inf, sup));
        for (size_t i = 0; i < columns * lines; i++)
        {
            _Matrixptr[i] = distribution(engine);
        }
    }

    Matrix(size_t lines, size_t columns, double inf, double sup) : _lines(lines), _columns(columns) {
        _Matrixptr = new T[lines * columns];
        std::random_device engine;
        std::uniform_real_distribution distribution(std::min(inf, sup), std::max(inf, sup));
        for (size_t i = 0; i < columns * lines; i++)
        {
            _Matrixptr[i] = distribution(engine);
        }
    }

    Matrix(size_t lines, size_t columns, complex<float> inf, complex<float> sup) : _lines(lines), _columns(columns) {
        _Matrixptr = new T[lines * columns];
        std::random_device engine;
        std::uniform_real_distribution distributionre(std::min(sup.real(), inf.real()), std::max(sup.real(), inf.real()));
        std::uniform_real_distribution distributionim(std::min(sup.imag(), inf.imag()), std::max(sup.imag(), inf.imag()));
        for (size_t i = 0; i < columns * lines; i++)
        {
            _Matrixptr[i] = std::complex<float>(distributionre(engine), distributionim(engine));
        }
    }

    Matrix(size_t lines, size_t columns, complex<double> inf, complex<double> sup) : _lines(lines), _columns(columns) {
        _Matrixptr = new T[lines * columns];
        std::random_device engine;
        std::uniform_real_distribution distributionre(std::min(sup.real(), inf.real()), std::max(sup.real(), inf.real()));
        std::uniform_real_distribution distributionim(std::min(sup.imag(), inf.imag()), std::max(sup.imag(), inf.imag()));
        for (size_t i = 0; i < columns * lines; i++)
        {
            _Matrixptr[i] = std::complex<double>(distributionre(engine), distributionim(engine));
        }
    }

    Matrix(const Matrix& other) :_lines(other._lines), _columns(other._columns)
    {
        if (other._Matrixptr == nullptr) {
            _Matrixptr = nullptr;
        }
        else {
            _Matrixptr = new T[_lines * _columns];
            for (size_t i = 0; i < other._lines * other._columns; i++) {
                _Matrixptr[i] = other._Matrixptr[i];
            }
        }
    }

    bool operator==(const Matrix<T>& rhs) const
    {
        if (_lines != rhs.getlines() || _columns != rhs.getcolumns())
        {
            return false;
        }
        for (size_t i = 0; i < _columns * _lines; i++)
        {
            double difference = abs(static_cast<double>(_Matrixptr[i]) - static_cast<double>(rhs._Matrixptr[i]));
            if (difference > epsilon)
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix<T>& rhs) const
    {
        return !(*this == rhs);
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            delete[] _Matrixptr;
            _Matrixptr = nullptr;

            _lines = other._lines;
            _columns = other._columns;

            if (other._Matrixptr != nullptr && _lines > 0 && _columns > 0) {
                _Matrixptr = new T[_lines * _columns];
                for (size_t i = 0; i < _lines * _columns; i++) {
                    _Matrixptr[i] = other._Matrixptr[i];
                }
            }
        }
        return *this;
    }

    size_t getlines() const {
        return _lines;
    }

    size_t getcolumns() const {
        return _columns;
    }

    T* data() {
        return _Matrixptr;
    }

    const T* data() const {
        return _Matrixptr;
    }

    T operator()(int line, int column) const {
        if (line < 0 || line >= static_cast<int>(_lines) || column < 0 || column >= static_cast<int>(_columns)) {
            throw std::range_error("Index out of range");
        }
        return _Matrixptr[line * _columns + column];
    }

    T& operator()(int line, int column) {
        if (line < 0 || line >= static_cast<int>(_lines) || column < 0 || column >= static_cast<int>(_columns)) {
            throw std::range_error("Index out of range");
        }
        return _Matrixptr[line * _columns + column];
    }

    Matrix& operator+=(const Matrix rhs) {
        if (_lines != rhs.getlines() || _columns != rhs.getcolumns()) {
            throw invalid_argument("Different size of Matrix");
        }
        for (size_t i = 0; i < _lines * _columns; i++) {
            _Matrixptr[i] += rhs._Matrixptr[i];
        }
        return *this;
    }

    Matrix operator+(const Matrix& rhs) {
        if (_lines != rhs.getlines() || _columns != rhs.getcolumns()) {
            throw invalid_argument("Different size of Matrix");
        }
        Matrix result(*this);
        result += rhs;
        return result;
    }

    Matrix& operator-=(const Matrix rhs) {
        if (_lines != rhs.getlines() || _columns != rhs.getcolumns()) {
            throw invalid_argument("Different size of Matrix");
        }
        for (size_t i = 0; i < _lines * _columns; i++) {
            _Matrixptr[i] -= rhs._Matrixptr[i];
        }
        return *this;
    }

    Matrix operator-(const Matrix& rhs) {
        if (_lines != rhs.getlines() || _columns != rhs.getcolumns()) {
            throw invalid_argument("Different size of Matrix");
        }
        Matrix result(*this);
        result -= rhs;
        return result;
    }

    Matrix operator*(T scalar) const {
        Matrix result(_lines, _columns, 0);
        for (size_t i = 0; i < _lines * _columns; i++) {
            result._Matrixptr[i] = _Matrixptr[i] * scalar;
        }
        return result;
    }

    friend Matrix operator*(T scalar, const Matrix& rhs) {
        return rhs * scalar;
    }

    Matrix operator/(T scalar) const {
        if (scalar == 0) {
            throw invalid_argument("Division by zero");
        }
        Matrix result(_lines, _columns, 0);
        for (int i = 0; i < static_cast<int>(_lines * _columns); i++) {
            result._Matrixptr[i] = _Matrixptr[i] / scalar;
        }
        return result;
    }

    T trace() const {
        T trace = 0;
        for (size_t i = 0; i < _lines; i++) {
            for (size_t j = 0; j < _columns; j++) {
                if (i == j) {
                    trace += this->operator()(static_cast<int>(i), static_cast<int>(j));
                }
            }
        }
        return trace;
    }

    Matrix operator*(const Matrix& rhs) const {
        if (_columns != rhs._lines) {
            throw invalid_argument("Uncorrect size of matrix for multiply");
        }
        Matrix<T> result(_lines, rhs._columns, 0);
        for (size_t i = 0; i < _lines; i++) {
            for (size_t j = 0; j < rhs._columns; j++) {
                for (size_t k = 0; k < _columns; k++) {
                    result(static_cast<int>(i), static_cast<int>(j)) += this->operator()(static_cast<int>(i), static_cast<int>(k)) *
                        rhs(static_cast<int>(k), static_cast<int>(j));
                }
            }
        }
        return result;
    }

    ~Matrix() {
        delete[] _Matrixptr;
    }
};

inline bool Matrix<std::complex<float> >::operator==(const Matrix<std::complex<float> >& rhs) const {
    if (_lines != rhs._lines || _columns != rhs._columns)
    {
        return false;
    }
    for (size_t i = 0; i < _columns * _lines; i++)
    {
        double difference_re = abs((double)_Matrixptr[i].real() - (double)rhs._Matrixptr[i].real());
        double difference_im = abs((double)_Matrixptr[i].imag() - (double)rhs._Matrixptr[i].imag());
        if (difference_re > epsilon || difference_im > epsilon)
        {
            return false;
        }
    }
    return true;
}

inline bool Matrix<std::complex<double> >::operator==(const Matrix<std::complex<double> >& rhs) const {
    if (_lines != rhs._lines || _columns != rhs._columns)
    {
        return false;
    }
    for (size_t i = 0; i < _columns * _lines; i++)
    {
        double difference_re = abs((double)_Matrixptr[i].real() - (double)rhs._Matrixptr[i].real());
        double difference_im = abs((double)_Matrixptr[i].imag() - (double)rhs._Matrixptr[i].imag());
        if (difference_re > epsilon || difference_im > epsilon)
        {
            return false;
        }
    }
    return true;
}

template <typename T>
Matrix<T> multiply_matrix_parallel(const Matrix<T>& a, const Matrix<T>& b, int rank, int size) {
    if (a.getcolumns() != b.getlines()) {
        throw invalid_argument("Matrix dimensions mismatch for multiplication");
    }

    size_t n = a.getlines();

    size_t rows_per_proc = n / size;
    size_t remainder = n % size;

    size_t start_row, end_row;
    if (rank < static_cast<int>(remainder)) {
        start_row = rank * (rows_per_proc + 1);
        end_row = start_row + rows_per_proc + 1;
    }
    else {
        start_row = rank * rows_per_proc + remainder;
        end_row = start_row + rows_per_proc;
    }

    size_t local_rows = end_row - start_row;

    Matrix<T> local_result(local_rows, b.getcolumns(), 0);

    for (size_t i = 0; i < local_rows; i++) {
        size_t global_i = start_row + i;
        for (size_t j = 0; j < b.getcolumns(); j++) {
            T sum = 0;
            for (size_t k = 0; k < a.getcolumns(); k++) {
                sum += a(static_cast<int>(global_i), static_cast<int>(k)) *
                    b(static_cast<int>(k), static_cast<int>(j));
            }
            local_result(static_cast<int>(i), static_cast<int>(j)) = sum;
        }
    }

    int* recvcounts = nullptr;
    int* displs = nullptr;
    Matrix<T> full_result;

    if (rank == 0) {
        full_result = Matrix<T>(n, b.getcolumns(), 0);
        recvcounts = new int[size];
        displs = new int[size];

        int offset = 0;
        for (int p = 0; p < size; ++p) {
            size_t p_rows = (p < static_cast<int>(remainder)) ? rows_per_proc + 1 : rows_per_proc;
            recvcounts[p] = static_cast<int>(p_rows * b.getcolumns());
            displs[p] = offset;
            offset += recvcounts[p];
        }
    }

    MPI_Gatherv(local_result.data(), static_cast<int>(local_rows * b.getcolumns()), MPI_INT,
        (rank == 0) ? full_result.data() : nullptr,
        recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        delete[] recvcounts;
        delete[] displs;
    }

    return full_result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& Matrix)
{
    for (size_t i = 0; i < Matrix.getlines(); i++) {
        for (size_t j = 0; j < Matrix.getcolumns(); j++) {
            os << Matrix(static_cast<int>(i), static_cast<int>(j)) << " ";
        }
        os << "\n";
    }
    return os;
}

#endif