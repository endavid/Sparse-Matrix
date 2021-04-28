/**
 * This file is part of the SparseMatrix library
 *
 * @license  MIT
 * @author   Petr Kessler (https://kesspess.cz)
 * @link     https://github.com/uestla/Sparse-Matrix
 */

#ifndef __TESTSLIB_H__

	#define	__TESTSLIB_H__

	#include <string>
	#include <vector>
	#include <cstring>
	#include <sstream>
	#include <iostream>
	#include <typeinfo>
	#include <exception>
    #include "helpers.h"
    #include "../../src/SparseMatrix/SparseMatrix.h"



	class FailureException : public std::exception
	{

		public:

			FailureException(const std::string & message) : std::exception(), message(message)
			{}


			virtual ~FailureException(void) throw ()
			{}


			inline std::string getMessage(void) const
			{
				return this->message;
			}


		protected:

			std::string message;

	};

    void assertException(const char * exceptionClass, void (*callback)(void));

    template<typename T>
    bool operator == (const Sparse::SparseMatrix<T> & sparse, const std::vector<std::vector<T> > & classical)
    {
        for (int i = 0, rows = classical.size(); i < rows; i++) {
            for (int j = 0, cols = classical[i].size(); j < cols; j++) {
                if (sparse.get(i + 1, j + 1) != classical[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

	template<typename T>
	void assertEquals(const T & a, const T & b, const char * message = NULL)
	{
		if (!(a == b)) {
			std::ostringstream oss;
			if (message == NULL) {
				oss << "Objects not equal when they should be." << std::endl;

			} else {
				oss << message << std::endl;
			}

			oss << a << std::endl << "expected, but" << std::endl << b << " given";
			throw FailureException(oss.str());
		}
	}


	template<typename X, typename Y>
	void assertEquals(const X & a, const Y & b, const char * message = NULL)
	{
		if (!(a == b)) {
			std::ostringstream oss;
			if (message == NULL) {
				oss << "Objects not equal when they should be." << std::endl;

			} else {
				oss << message << std::endl;
			}

			oss << a << std::endl << "expected, but" << std::endl << b << " given";
			throw FailureException(oss.str());
		}
	}


#endif
