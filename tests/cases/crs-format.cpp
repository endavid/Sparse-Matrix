/**
 * This file is part of the SparseMatrix library
 *
 * @license  MIT
 * @author   Petr Kessler (https://kesspess.cz)
 * @link     https://github.com/uestla/Sparse-Matrix
 */

#include "../inc/testslib.h"
#include "../inc/SparseMatrixMock.h"


void testInternalStorage(void)
{
	std::cout << "internal storage..." << std::flush;

	/*
		"Standard" matrix
		[ 1  0 4 5 ]
		[ 2 -1 0 0 ]
		[ 0  0 3 2 ]

		should be stored as
		rows:    [ 0, 3, 5, 7 ]           (index in values vector that start a row)
		columns: [ 0, 2, 3, 0, 1, 2, 3 ]  (column indices in values vector)
		values:  [ 1, 4, 5, 2, -1, 3, 2 ]
	*/

	SparseMatrixMock<int> m1(3, 4);
	m1.set(1, 0, 0)
		.set(4, 0, 2)
		.set(5, 0, 3)
		.set(2, 1, 0)
		.set(-1, 1, 1)
		.set(3, 2, 2)
		.set(2, 2, 3);

    std::vector<size_t> rowPointers1 { 0, 3, 5, 7 };
	assertEquals<std::vector<size_t> >(rowPointers1, *(m1.getRowPointers()), "Incorrect internal row pointers");

    std::vector<size_t> columnPointers1 { 0, 2, 3, 0, 1, 2, 3 };
	assertEquals<std::vector<size_t> >(columnPointers1, *(m1.getColumnPointers()), "Incorrect internal column pointers");

    std::vector<int> values1 {1, 4, 5, 2, -1, 3, 2 };
	assertEquals<std::vector<int> >(values1, *(m1.getValues()), "Incorrect internal values storage");


	/*
		Matrix with empty row
		[ 10 0 0 2 ]
		[  0 0 0 0 ]
		[  3 1 0 4 ]

		should be stored as
		rows:    [ 0, 2, 2, 5 ]
		columns: [ 0, 3, 0, 1, 3 ]
		values:  [ 10, 2, 3, 1, 4 ]
	*/

	SparseMatrixMock<int> m2(3, 4);
	m2.set(10, 0, 0)
		. set(2, 0, 3)
		. set(3, 2, 0)
		. set(1, 2, 1)
		. set(4, 2, 3);

    std::vector<size_t> rowPointers2 { 0, 2, 2, 5 };
	assertEquals<std::vector<size_t> >(rowPointers2, *(m2.getRowPointers()), "Incorrect internal row pointers");

    std::vector<size_t> columnPointers2 { 0, 3, 0, 1, 3 };
	assertEquals<std::vector<size_t> >(columnPointers2, *(m2.getColumnPointers()), "Incorrect internal column pointers");

    std::vector<int> values2 { 10, 2, 3, 1, 4 };
	assertEquals<std::vector<int> >(values2, *(m2.getValues()), "Incorrect internal values storage");


	/*
		Previous matrix after adding non-zero element to empty row

		should be stored as
		rows:    [ 0, 2, 3, 6 ]
		columns: [ 0, 3, 1, 0, 1, 3 ]
		values:  [ 10, 2, 5, 3, 1, 4 ]
	 */

	SparseMatrixMock<int> m3 = m2;
	m3.set(5, 1, 1);

    std::vector<size_t> rowPointers3 {0, 2, 3, 6};
	assertEquals<std::vector<size_t> >(rowPointers3, *(m3.getRowPointers()), "Incorrect internal row pointers");

    std::vector<size_t> columnPointers3 { 0, 3, 1, 0, 1, 3 };
	assertEquals<std::vector<size_t> >(columnPointers3, *(m3.getColumnPointers()), "Incorrect internal column pointers");

    std::vector<int> values3 { 10, 2, 5, 3, 1, 4 };
	assertEquals<std::vector<int> >(values3, *(m3.getValues()), "Incorrect internal values storage");

	/*
		Previous matrix with removed the only non-zero element on 2nd row (should be equal to 2nd matrix)

		should be stored as
		rows:    [ 0, 2, 2, 5 ]
		columns: [ 0, 3, 0, 1, 3 ]
		values:  [ 10, 2, 3, 1, 4 ]
	 */

	SparseMatrixMock<int> m4 = m3;
	m4.set(0, 1, 1);

	assertEquals<std::vector<size_t> >(rowPointers2, *(m4.getRowPointers()), "Incorrect internal row pointers");
	assertEquals<std::vector<size_t> >(columnPointers2, *(m4.getColumnPointers()), "Incorrect internal column pointers");
	assertEquals<std::vector<int> >(values2, *(m4.getValues()), "Incorrect internal values storage");

	std::cout << " OK" << std::endl;
}

void testColumnMatrix()
{
    std::cout << "column matrix..." << std::flush;

    /*
     [ 1  0 4 5 ]
     [ 2 -1 0 0 ]
     [ 0  0 3 2 ]
     */
    SparseMatrixMock<int> m1(3, 4);
    m1.set(1, 0, 0)
        .set(4, 0, 2)
        .set(5, 0, 3)
        .set(2, 1, 0)
        .set(-1, 1, 1)
        .set(3, 2, 2)
        .set(2, 2, 3);
    
    SparseMatrixMock<int> c2(m1.getColumn(2));
    
    std::vector<int> values2 { 4, 3 };
    assertEquals<std::vector<int> >(values2, *(c2.getValues()), "Incorrect internal values storage");
    std::vector<size_t> rowPointers2 { 0, 1, 1, 2 };
    assertEquals<std::vector<size_t> >(rowPointers2, *(c2.getRowPointers()), "Incorrect internal row pointers");
    std::vector<size_t> columnPointers2 { 0, 0 };
    assertEquals<std::vector<size_t> >(columnPointers2, *(c2.getColumnPointers()), "Incorrect internal column pointers");

    // [ 4 0 3 ]
    SparseMatrixMock<int> c2t(m1.getColumnTransposed(2));
    
    assertEquals<std::vector<int> >(values2, *(c2t.getValues()), "Incorrect internal values storage");
    std::vector<size_t> rowPointers2t { 0, 2 };
    assertEquals<std::vector<size_t> >(rowPointers2t, *(c2t.getRowPointers()), "Incorrect internal row pointers");
    std::vector<size_t> columnPointers2t { 0, 2 };
    assertEquals<std::vector<size_t> >(columnPointers2t, *(c2t.getColumnPointers()), "Incorrect internal column pointers");
    
    /*
     [16 0 12]
     [0  0  0]
     [12 0  9]
     */
    SparseMatrixMock<int> xx(c2 * c2t);
    std::vector<int> valuesxx { 16, 12, 12, 9 };
    assertEquals<std::vector<int> >(valuesxx, *(xx.getValues()), "Incorrect internal values storage");
    std::vector<size_t> rowPointersxx { 0, 2, 2, 4 };
    assertEquals<std::vector<size_t> >(rowPointersxx, *(xx.getRowPointers()), "Incorrect internal row pointers");
    std::vector<size_t> columnPointersxx { 0, 2, 0, 2 };
    assertEquals<std::vector<size_t> >(columnPointersxx, *(xx.getColumnPointers()), "Incorrect internal column pointers");

}
