/**
 * This file is part of the SparseMatrix library
 *
 * @license  MIT
 * @author   Petr Kessler (https://kesspess.cz)
 * @link     https://github.com/uestla/Sparse-Matrix
 */

#include "../inc/testslib.h"
#include "../inc/helpers.h"
#include "../inc/SparseMatrixMock.h"


void _additionFail1(void)
{
	Sparse::SparseMatrix<int> a(3, 4), b(3, 5);
	a.add(b);
}


void testAdditionFail1(void)
{
	std::cout << "add() fail #1..." << std::flush;
	assertException("InvalidDimensionsException", _additionFail1);
	std::cout << " OK" << std::endl;
}


void _additionFail2(void)
{
	Sparse::SparseMatrix<int> a(3, 4), b(4, 4);
	a.add(b);
}


void testAdditionFail2(void)
{
	std::cout << "add() fail #2..." << std::flush;
	assertException("InvalidDimensionsException", _additionFail2);
	std::cout << " OK" << std::endl;
}


void _additionFail3(void)
{
	Sparse::SparseMatrix<int> a(3, 4), b(4, 5);
	a.add(b);
}


void testAdditionFail3(void)
{
	std::cout << "add() fail #3..." << std::flush;
	assertException("InvalidDimensionsException", _additionFail3);
	std::cout << " OK" << std::endl;
}


void testAddition(void)
{
	for (int N = 0; N < 5e3; N++) {
		std::cout << "\rmatrices addition... #" << N + 1 << std::flush;

		// generate random matrices
		int rows = rand() % 16 + 1;
		int cols = rand() % 16 + 1;

		std::vector<std::vector<int> > classicMatrixA = generateRandomMatrix<int>(rows, cols);
		SparseMatrixMock<int> sparseMatrixA = SparseMatrixMock<int>::fromVectors(classicMatrixA);

		std::vector<std::vector<int> > classicMatrixB = generateRandomMatrix<int>(rows, cols);
		SparseMatrixMock<int> sparseMatrixB = SparseMatrixMock<int>::fromVectors(classicMatrixB);

		// calculate result manually
		std::vector<std::vector<int> > manualResult = addMatrices(classicMatrixA, classicMatrixB);

		// method
		assertEquals<Sparse::SparseMatrix<int>, std::vector<std::vector<int> > >(
			sparseMatrixA.add(sparseMatrixB),
			manualResult,
			"Incorrect matrices addition"
		);

		// operator
		assertEquals<Sparse::SparseMatrix<int>, std::vector<std::vector<int> > >(
			sparseMatrixA + sparseMatrixB,
			manualResult,
			"Incorrect matrices addition (operator +)"
		);
	}

	std::cout << " OK" << std::endl;
}

void testAddSubmatrix()
{
    /*
     [ 2 0 2 0 .... 2 ]
     [ 0 ...        0 ]
     [ 0 ...        0 ]
     [ 2 0 2 0 .... 2 ]
     [ ...            ]
     */
    SparseMatrixMock<int> matrix(6, 12);
    for (size_t i = 0; i < matrix.getRowCount(); i+=3)
    {
        for (size_t j = 0; j < matrix.getColumnCount(); j+=2)
        {
            matrix.set(2, i, j);
        }
    }
    SparseMatrixMock<int> c2(matrix.getColumn(2));
    std::vector<std::vector<int> > c2expected = {
        {2}, {0}, {0}, {2}, {0}, {0}
    };
    assertEquals<Sparse::SparseMatrix<int>, std::vector<std::vector<int> > >(c2, c2expected, "Unexpected column matrix");
    SparseMatrixMock<int> xx(c2 * matrix.getColumnTransposed(2));
    std::vector<std::vector<int> > xxexpected = {
        {4, 0, 0, 4, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {4, 0, 0, 4, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}
    };
    assertEquals<Sparse::SparseMatrix<int>, std::vector<std::vector<int> > >(xx, xxexpected, "Unexpected matrix");
    
    matrix.addSubmatrix(xx);
    std::vector<std::vector<int> > expected = {
        {6, 0, 2, 4, 2, 0, 2, 0, 2, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {6, 0, 2, 4, 2, 0, 2, 0, 2, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    assertEquals<Sparse::SparseMatrix<int>, std::vector<std::vector<int> > >(matrix, expected, "Unexpected matrix");
}
