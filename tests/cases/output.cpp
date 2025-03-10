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
	std::cout << "output..." << std::flush;

	std::ostringstream oss;
	std::string output;

	Sparse::SparseMatrix<int> m(3, 3);
	oss << m;
	assertEquals<std::string>("0 0 0\n0 0 0\n0 0 0", oss.str());

	m.set(7, 0, 2)
		.set(5, 1, 1)
		.set(3, 2, 0);

	oss.str("");
	oss << m;
	assertEquals<std::string>("0 0 7\n0 5 0\n3 0 0", oss.str());

	std::cout << " OK" << std::endl;
}
