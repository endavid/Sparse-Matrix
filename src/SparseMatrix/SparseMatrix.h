/**
 * This file is part of the SparseMatrix library
 *
 * @license  MIT
 * @author   Petr Kessler (https://kesspess.cz)
 * @link     https://github.com/uestla/Sparse-Matrix
 */

#ifndef __SPARSEMATRIX_H__

	#define	__SPARSEMATRIX_H__

	#include <vector>
	#include <iostream>
    #include "exceptions.h"


	namespace Sparse
	{

		template<typename T>
		class SparseMatrix
		{

			public:

				// === CREATION ==============================================

				SparseMatrix(size_t n); // square matrix n×n
				SparseMatrix(size_t rows, size_t columns); // general matrix

				SparseMatrix(const SparseMatrix<T> & m); // copy constructor
				SparseMatrix<T> & operator = (const SparseMatrix<T> & m);

				~SparseMatrix(void);


				// === GETTERS / SETTERS ==============================================

                size_t getRowCount(void) const;
                size_t getColumnCount(void) const;


				// === VALUES ==============================================

				T get(size_t row, size_t col) const;
				SparseMatrix & set(T val, size_t row, size_t col);
                SparseMatrix<T> getColumn(size_t col);
                SparseMatrix<T> getColumnTransposed(size_t col);


				// === OPERATIONS ==============================================

				std::vector<T> multiply(const std::vector<T> & x) const;
				std::vector<T> operator * (const std::vector<T> & x) const;

				SparseMatrix<T> multiply(const SparseMatrix<T> & m) const;
				SparseMatrix<T> operator * (const SparseMatrix<T> & m) const;

				SparseMatrix<T> add(const SparseMatrix<T> & m) const;
				SparseMatrix<T> operator + (const SparseMatrix<T> & m) const;

				SparseMatrix<T> subtract(const SparseMatrix<T> & m) const;
				SparseMatrix<T> operator - (const SparseMatrix<T> & m) const;

                void addSubmatrix(const SparseMatrix<T> & m);


				// === FRIEND FUNCTIONS =========================================

				template<typename X>
				friend bool operator == (const SparseMatrix<X> & a, const SparseMatrix<X> & b);

				template<typename X>
				friend bool operator != (const SparseMatrix<X> & a, const SparseMatrix<X> & b);

				template<typename X>
				friend std::ostream & operator << (std::ostream & os, const SparseMatrix<X> & matrix);


			protected:

                size_t m, n;

				std::vector<T> * vals;
				std::vector<size_t> * rows, * cols;


				// === HELPERS / VALIDATORS ==============================================

				void construct(size_t m, size_t n);
				void destruct(void);
				void deepCopy(const SparseMatrix<T> & m);
				void validateCoordinates(size_t row, size_t col) const;
				void insert(size_t index, size_t row, size_t col, T val);
				void remove(size_t index, size_t row);

		};

    // === CREATION ==============================================

    template<typename T>
    SparseMatrix<T>::SparseMatrix(size_t n)
    {
        this->construct(n, n);
    }


    template<typename T>
    SparseMatrix<T>::SparseMatrix(size_t rows, size_t columns)
    {
        this->construct(rows, columns);
    }


    template<typename T>
    SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> & matrix)
    {
        this->deepCopy(matrix);
    }


    template<typename T>
    SparseMatrix<T> & SparseMatrix<T>::operator = (const SparseMatrix<T> & matrix)
    {
        if (&matrix != this) {
            this->destruct();
            this->deepCopy(matrix);
        }

        return *this;
    }


    template<typename T>
    void SparseMatrix<T>::deepCopy(const SparseMatrix<T> & matrix)
    {
        this->m = matrix.m;
        this->n = matrix.n;
        this->rows = new std::vector<size_t>(*(matrix.rows));

        if (matrix.vals != nullptr) {
            this->cols = new std::vector<size_t>(*(matrix.cols));
            this->vals = new std::vector<T>(*(matrix.vals));
        }
    }


    template<typename T>
    SparseMatrix<T>::~SparseMatrix(void)
    {
        this->destruct();
    }


    template<typename T>
    void SparseMatrix<T>::construct(size_t rows, size_t columns)
    {
        if (rows < 1 || columns < 1) {
            throw InvalidDimensionsException("Matrix dimensions cannot be zero or negative.");
        }

        this->m = rows;
        this->n = columns;

        this->vals = nullptr;
        this->cols = nullptr;
        this->rows = new std::vector<size_t>(rows + 1, 0);
    }


    template<typename T>
    void SparseMatrix<T>::destruct(void)
    {
        if (this->vals != nullptr) {
            delete this->vals;
            delete this->cols;
        }

        delete this->rows;
    }


    // === GETTERS / SETTERS ==============================================

    template<typename T>
    size_t SparseMatrix<T>::getRowCount(void) const
    {
        return this->m;
    }


    template<typename T>
    size_t SparseMatrix<T>::getColumnCount(void) const
    {
        return this->n;
    }


    // === VALUES ==============================================

    template<typename T>
    T SparseMatrix<T>::get(size_t row, size_t col) const
    {
        this->validateCoordinates(row, col);

        size_t currCol;

        for (size_t pos = (*(this->rows))[row]; pos < (*(this->rows))[row + 1]; ++pos) {
            currCol = (*(this->cols))[pos];

            if (currCol == col) {
                return (*(this->vals))[pos];

            } else if (currCol > col) {
                break;
            }
        }

        return T();
    }


    template<typename T>
    SparseMatrix<T> & SparseMatrix<T>::set(T val, size_t row, size_t col)
    {
        this->validateCoordinates(row, col);

        size_t pos = (*(this->rows))[row];
        size_t currCol = -1;

        for (; pos < (*(this->rows))[row + 1]; pos++) {
            currCol = (*(this->cols))[pos];

            if (currCol >= col) {
                break;
            }
        }

        if (currCol != col) {
            if (!(val == T())) {
                this->insert(pos, row, col, val);
            }

        } else if (val == T()) {
            this->remove(pos, row);

        } else {
            (*(this->vals))[pos] = val;
        }

        return *this;
    }

    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::getColumn(size_t col)
    {
        this->validateCoordinates(0, col);
        SparseMatrix<T> outM(this->m, 1);

        if (this->vals != nullptr) { // only if any value set
            for (size_t i = 0; i < this->m; i++) {
                for (size_t pos = (*(this->rows))[i]; pos < (*(this->rows))[i + 1]; pos++) {
                    size_t j = (*(this->cols))[pos];
                    if (col == j) {
                        auto val = (*(this->vals))[pos];
                        outM.set(val, i, 0);
                    }
                }
            }
        }
        return outM;
    }
    
    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::getColumnTransposed(size_t col)
    {
        this->validateCoordinates(0, col);
        SparseMatrix<T> outM(1, this->m);

        if (this->vals != nullptr) { // only if any value set
            for (size_t i = 0; i < this->m; i++) {
                for (size_t pos = (*(this->rows))[i]; pos < (*(this->rows))[i + 1]; pos++) {
                    size_t j = (*(this->cols))[pos];
                    if (col == j) {
                        auto val = (*(this->vals))[pos];
                        outM.set(val, 0, i);
                    }
                }
            }
        }
        return outM;
    }

    // === OPERATIONS ==============================================

    template<typename T>
    std::vector<T> SparseMatrix<T>::multiply(const std::vector<T> & x) const
    {
        if (this->n != (int) x.size()) {
            throw InvalidDimensionsException("Cannot multiply: Matrix column count and vector size don't match.");
        }

        std::vector<T> result(this->m, T());

        if (this->vals != nullptr) { // only if any value set
            for (size_t i = 0; i < this->m; i++) {
                T sum = T();
                for (size_t j = (*(this->rows))[i]; j < (*(this->rows))[i + 1]; j++) {
                    sum = sum + (*(this->vals))[j] * x[(*(this->cols))[j]];
                }

                result[i] = sum;
            }
        }

        return result;
    }


    template<typename T>
    std::vector<T> SparseMatrix<T>::operator * (const std::vector<T> & x) const
    {
        return this->multiply(x);
    }


    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::multiply(const SparseMatrix<T> & m) const
    {
        if (this->n != m.m) {
            throw InvalidDimensionsException("Cannot multiply: Left matrix column count and right matrix row count don't match.");
        }

        SparseMatrix<T> result(this->m, m.n);

        T a;

        // TODO: more efficient?
        // @see http://www.math.tamu.edu/~srobertp/Courses/Math639_2014_Sp/CRSDescription/CRSStuff.pdf

        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < m.n; j++) {
                a = T();

                for (size_t k = 0; k < this->n; k++) {
                    a = a + this->get(i, k) * m.get(k, j);
                }

                result.set(a, i, j);
            }
        }

        return result;
    }


    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::operator * (const SparseMatrix<T> & m) const
    {
        return this->multiply(m);
    }


    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::add(const SparseMatrix<T> & m) const
    {
        if (this->m != m.m || this->n != m.n) {
            throw InvalidDimensionsException("Cannot add: matrices dimensions don't match.");
        }

        SparseMatrix<T> result(this->m, this->n);

        // TODO: more efficient?
        // @see http://www.math.tamu.edu/~srobertp/Courses/Math639_2014_Sp/CRSDescription/CRSStuff.pdf

        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < this->n; j++) {
                result.set(this->get(i, j) + m.get(i, j), i, j);
            }
        }

        return result;
    }


    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::operator + (const SparseMatrix<T> & m) const
    {
        return this->add(m);
    }

    template<typename T>
    void SparseMatrix<T>::addSubmatrix(const SparseMatrix<T> & m)
    {
        if (this->m < m.m || this->n < m.n) {
            throw InvalidDimensionsException("Cannot add submatrix with bigger dimension.");
        }

        for (size_t i = 0; i < m.m; i++) {
            for (size_t j = 0; j < m.n; j++) {
                this->set(this->get(i, j) + m.get(i, j), i, j);
            }
        }
    }

    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::subtract(const SparseMatrix<T> & m) const
    {
        if (this->m != m.m || this->n != m.n) {
            throw InvalidDimensionsException("Cannot subtract: matrices dimensions don't match.");
        }

        SparseMatrix<T> result(this->m, this->n);

        // TODO: more efficient?
        // @see http://www.math.tamu.edu/~srobertp/Courses/Math639_2014_Sp/CRSDescription/CRSStuff.pdf

        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < this->n; j++) {
                result.set(this->get(i, j) - m.get(i, j), i, j);
            }
        }

        return result;
    }


    template<typename T>
    SparseMatrix<T> SparseMatrix<T>::operator - (const SparseMatrix<T> & m) const
    {
        return this->subtract(m);
    }


    // === HELPERS / VALIDATORS ==============================================

    template<typename T>
    void SparseMatrix<T>::validateCoordinates(size_t row, size_t col) const
    {
        if (row < 0 || col < 0 || row >= this->m || col >= this->n) {
            throw InvalidCoordinatesException("Coordinates out of range.");
        }
    }


    template<typename T>
    void SparseMatrix<T>::insert(size_t index, size_t row, size_t col, T val)
    {
        if (this->vals == nullptr) {
            this->vals = new std::vector<T>(1, val);
            this->cols = new std::vector<size_t>(1, col);

        } else {
            this->vals->insert(this->vals->begin() + index, val);
            this->cols->insert(this->cols->begin() + index, col);
        }

        for (size_t i = row + 1; i <= this->m; i++) {
            (*(this->rows))[i] += 1;
        }
    }


    template<typename T>
    void SparseMatrix<T>::remove(size_t index, size_t row)
    {
        this->vals->erase(this->vals->begin() + index);
        this->cols->erase(this->cols->begin() + index);

        for (size_t i = row + 1; i <= this->m; i++) {
            (*(this->rows))[i] -= 1;
        }
    }


    // === FRIEND FUNCTIONS =========================================

    template<typename T>
    bool operator == (const SparseMatrix<T> & a, const SparseMatrix<T> & b)
    {
        return ((a.vals == nullptr && b.vals == nullptr)
                    || (a.vals != nullptr && b.vals != nullptr && *(a.vals) == *(b.vals)))
                && ((a.cols == nullptr && b.cols == nullptr)
                    || (a.cols != nullptr && b.cols != nullptr && *(a.cols) == *(b.cols)))
                && *(a.rows) == *(b.rows);
    }


    template<typename T>
    bool operator != (const SparseMatrix<T> & a, const SparseMatrix<T> & b)
    {
        return !(a == b);
    }


    template<typename T>
    std::ostream & operator << (std::ostream & os, const SparseMatrix<T> & matrix)
    {
        for (size_t i = 0; i < matrix.m; i++) {
            for (size_t j = 0; j < matrix.n; j++) {
                if (j != 0) {
                    os << " ";
                }

                os << matrix.get(i, j);
            }

            if (i < matrix.m - 1) {
                os << std::endl;
            }
        }

        return os;
    }
    
	}
#endif
