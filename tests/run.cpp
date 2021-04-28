/**
 * This file is part of the SparseMatrix library
 *
 * @license  MIT
 * @author   Petr Kessler (https://kesspess.cz)
 * @link     https://github.com/uestla/Sparse-Matrix
 */

#include <ctime>
#include <cstdlib>
#include <iostream>
#include "inc/testslib.h"
#include "inc/helpers.h"

void testConstructorFail1();
void testConstructorFail2();
void testConstructorFail3();
void testConstructorFail4();
void testGetFail();
void testSetFail();
void testMultiplicationFail1();
void testMultiplicationFail2();
void testAdditionFail1();
void testAdditionFail2();
void testAdditionFail3();
void testSubtractionFail1();
void testSubtractionFail2();
void testSubtractionFail3();
void testGettersAndSetters();
void testInternalStorage();
void testColumnMatrix();
void testOutput();
void testVectorMultiplication();
void testMatricesMultiplication();
void testAddition();
void testSubtraction();
void testElementTypes();

int main(int argc, char ** argv)
{
    srand(static_cast<unsigned int>(time(nullptr))); // used at random matrices / vectors generating

	try {

		testConstructorFail1();
		testConstructorFail2();
		testConstructorFail3();
		testConstructorFail4();
		testGetFail();
		testSetFail();
		testMultiplicationFail1();
		testMultiplicationFail2();
		testAdditionFail1();
		testAdditionFail2();
		testAdditionFail3();
		testSubtractionFail1();
		testSubtractionFail2();
		testSubtractionFail3();
		testGettersAndSetters();
		testInternalStorage();
        testColumnMatrix();
		testOutput();
		testVectorMultiplication();
		testMatricesMultiplication();
		testAddition();
		testSubtraction();
		testElementTypes();

	} catch (const FailureException & e) {
		std::cout << " - FAIL: '" << e.getMessage() << "'" << std::endl;
		return 1;
	}

	return 0;
}
