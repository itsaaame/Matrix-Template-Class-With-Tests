#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

template <typename T> class Matrix
{
public:

    Matrix(size_t rows, size_t columns)
        : m_rows(rows), m_columns(columns), m_elements(rows* columns) {}

    Matrix(size_t rows, size_t columns, T value)
        : m_rows(rows), m_columns(columns), m_elements(rows* columns, value) {}

    Matrix(size_t rows, size_t columns, std::initializer_list<T> values)
        : m_rows(rows), m_columns(columns), m_elements(rows* columns) {
        if (rows * columns < values.size())
        {
            throw std::range_error("Error: wrong matrix shape!");
        }
        std::copy(values.begin(), values.end(), m_elements.begin());
    }

    size_t rows() const { return m_rows; }

    size_t columns() const { return m_columns; }

    // matrix elemetns are contained inside vector for easier of use
    // and access to vector built-in methods if need be
    T& operator()(size_t row, size_t column) {
        if (row >= m_rows || column >= m_columns)
        {
            throw std::range_error("Error: dimensions are outside of matrix range!");
        }

        // in order to get the desired elements we need to do a little calculation of where
        // it would be located inside the matrix 
        return m_elements[row * m_columns + column];
    }

    const T& operator()(size_t row, size_t column) const {
        if (row >= m_rows || column >= m_columns)
        {
            throw std::range_error("Error: dimensions are outside of matrix range!");
        }
        return m_elements[row * m_columns + column];
    }

    Matrix<T> operator+(Matrix<T>& m) {
        if (m.rows() != m_rows || m.columns() != m_columns) {
            throw std::range_error("Error: matrix shapes do not match!");
        }
        Matrix<T> result(m_rows, m_columns);
        for (size_t i = 0; i < m_rows; i++)
        {
            for (size_t j = 0; j < m_columns; j++)
            {
                result(i,j) = (*this)(i,j) + m(i, j);
            }
        }
        return result;
    }

    Matrix<T> operator*(Matrix<T>& m) {
        if (m_columns != m.rows() || m_rows != m.columns())
        {
            throw std::range_error("Error: matrix shapes do not match!");
        }
        Matrix<T> result(m_rows, m.columns());
        for (size_t i = 0; i < m_rows; i++)
        {
            for (size_t j = 0; j < m.columns(); j++)
            {
                for (size_t k = 0; k < m_columns; k++)
                {
                    result(i,j) += (*this)(i,k) * m(k,j);
                }
            }
        }

        return result;
    }

    void swapRows(size_t i, size_t j){
        for (size_t column = 0; column < m_columns; ++column)
        {
            std::swap((*this)(i, column), (*this)(j, column));
        }
    }

    // Convert matrix to reduced row echelon form
    void rref() {
        for (size_t row = 0, lead = 0; row < m_rows && lead < m_columns; ++row, ++lead)
        {
            size_t i = row;
            while ((*this)(i, lead) == 0) 
            {
                if (++i == m_rows) 
                {
                    i = row;
                    if (++lead == m_columns)
                    {
                        return;
                    }
                }
            }
            this->swapRows(i, row);
            if ((*this)(row, lead) != 0) 
            {
                T f = (*this)(row, lead);
                for (size_t column = 0; column < m_columns; ++column)
                {
                    (*this)(row, column) /= f;
                }
            }
            for (size_t j = 0; j < m_rows; ++j) 
            {
                if (j == row)
                {
                    continue;
                }
                T f = (*this)(j, lead);
                for (size_t column = 0; column < m_columns; ++column)
                {
                    (*this)(j, column) -= f * (*this)(row, column);
                }
            }
        }
    }
    // matrix inverse calculated using Gauss-Jordan method
    Matrix<T> inverse() {
        if (m_rows != m_columns) 
        {
            throw std::range_error("Error: wrong matrix shape!");
        }
        Matrix<T> tmp(m_rows, 2 * m_rows, 0);
        for (size_t row = 0; row < m_rows; ++row)
        {
            for (size_t column = 0; column < m_rows; ++column)
            {
                tmp(row, column) = (*this)(row, column);
            }
            tmp(row, row + m_rows) = 1;
        }
        tmp.rref();
        Matrix<T> inv(m_rows, m_rows);
        for (size_t row = 0; row < m_rows; ++row)
        {
            for (size_t column = 0; column < m_rows; ++column)
            {
                inv(row, column) = tmp(row, column + m_rows);
            }
        }
        return inv;
    }

    void print(std::ostream& out) {
        out << std::fixed << std::setprecision(4);
        for (size_t row = 0; row < m_rows; ++row) 
        {
            for (size_t column = 0; column < m_columns; ++column) 
            {
                if (column > 0) 
                {
                    out << ' ';
                }
                out << std::setw(7) << m_elements[row * m_columns + column];
            }
            out << '\n';
        }
    }

    Matrix<T> transpose() {
        Matrix<T> result(m_columns, m_rows);
        for (size_t i = 0; i < m_rows; i++)
        {
            for (size_t j = 0; j < m_columns; j++)
            {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    void addRow(size_t row) {
        if (row > m_rows)
        {
            throw std::range_error("Error: dimensions are outside of matrix range!");
        }
        std::vector<T> empty(m_columns);
        m_elements.insert(m_elements.begin() + row * m_columns + m_columns,
            empty.begin(), empty.end());
        m_rows += 1;
    }

    void addRow(size_t row, std::initializer_list<T> values) {
        if (row > m_rows)
        {
            throw std::range_error("Error: dimensions are outside of matrix range!");
        }
        else if (values.size() > m_columns)
        {
            throw std::range_error("Error: list size of values is bigger than matrix dimension");
        }
        m_elements.insert(m_elements.begin() + row * m_columns + m_columns,
            values.begin(), values.end());
        m_rows += 1;
    }

    void addColumn(size_t column) {
        if (column > m_columns)
        {
            throw std::range_error("Error: dimensions are outside of matrix range!");
        }
        for (size_t i = 0; i < m_rows; i++)
        {
            m_elements.insert(m_elements.begin() + i*(m_columns + 1) + column + 1, 0);
        }
        m_columns += 1;
    }

    void addColumn(size_t column, std::initializer_list<T> values) {
        if (column > m_columns)
        {
            throw std::range_error("Error: dimensions are outside of matrix range!");
        }
        else if (values.size() > m_rows)
        {
            throw std::range_error("Error: list size of values is bigger than matrix dimension");
        }
        for (size_t i = 0; i < m_rows; i++)
        {
            m_elements.insert(m_elements.begin() + i * (m_columns + 1) + column + 1, 
                values.begin() + i, values.begin() + (i + 1));
        }
        m_columns += 1;
    }

private:
    size_t m_rows;
    size_t m_columns;
    std::vector<T> m_elements;
};

