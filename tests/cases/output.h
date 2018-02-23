/**
 * This file is part of the SparseMatrix library
 *
 * @license  MIT
 * @author   Petr Kessler (https://kesspess.cz)
 * @link     https://github.com/uestla/Sparse-Matrix
 */

#include "../inc/testslib.h"
#include "../inc/SparseMatrixMock.h"


void testOutput(void)
{
	cout << "output..." << flush;

	ostringstream oss;
	string output;

	SparseMatrix<int> m(3, 3);
	oss << m;
	assertEquals<string>("0 0 0\n0 0 0\n0 0 0", oss.str());

	m.set(7, 0, 2)
		.set(5, 1, 1)
		.set(3, 2, 0);

	oss.str("");
	oss << m;
	assertEquals<string>("0 0 7\n0 5 0\n3 0 0", oss.str());

	cout << " OK" << endl;
}
