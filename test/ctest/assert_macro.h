//
// Created by Howard Colin on 03/04/2023.
//

#ifndef CDL_ASSERT_MACRO_H
#define CDL_ASSERT_MACRO_H

#include <iostream>
#include <sstream>

#define assertEqual( ... )               \
do {                                            \
    if( !( __VA_ARGS__ ) ) {                     \
        std::cerr << "Unit test assert [ " \
        << ( #__VA_ARGS__ )             \
        << " ] failed in line [ "       \
        << __LINE__                     \
        << " ] file [ "                 \
        << __FILE__ << " ]"             \
        << std::endl;                     \
        err_code = 1;                           \
    }                                            \
} while( false )

#endif //CDL_ASSERT_MACRO_H
