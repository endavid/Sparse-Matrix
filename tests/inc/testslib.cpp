//
//  testslib.cpp
//  sparseMatrix
//
//  Created by David Gavilan on 2021/04/28.
//

#include <stdio.h>
#include "testslib.h"


void assertException(const char * exceptionClass, void (*callback)(void))
{
    try {
        callback();

    } catch (const std::exception & e) {
        std::string actualClass(typeid(e).name());

        if (strstr(actualClass.c_str(), exceptionClass) == NULL) {
            std::ostringstream oss;
            oss << "Exception class '" << exceptionClass << "' expected, but '" << actualClass << "' thrown.";

            throw FailureException(oss.str());
        }

        return ;
    }

    throw FailureException("Exception expected but none thrown.");
}
