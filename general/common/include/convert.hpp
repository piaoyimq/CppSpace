/*
 * convert.hpp
 *
 *  Created on: May 23, 2017
 *      Author: piaoyimq
 */

#ifndef GENERAL_COMMON_INCLUDE_CONVERT_HPP_
#define GENERAL_COMMON_INCLUDE_CONVERT_HPP_

#include <iostream>
#include <sstream>

template<class out_type, class in_value>
out_type convert(const in_value &t)
{
	out_type ans;
	std::stringstream a;
	a << t;
	a >> ans;
	return ans;
}

#endif /* GENERAL_COMMON_INCLUDE_CONVERT_HPP_ */
