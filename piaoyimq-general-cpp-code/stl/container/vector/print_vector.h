/*
 * print_vector.h
 *
 *  Created on: Sep 11, 2018
 *      Author: azhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_STL_CONTAINER_VECTOR_PRINT_VECTOR_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_STL_CONTAINER_VECTOR_PRINT_VECTOR_H_

//vector print
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
    s.put('[');
    char comma[3] = { '\0', ' ', '\0' };
    for(typename std::vector<T>::const_iterator it=v.begin(); it!=v.end(); ++it)
    {
        s << comma << *it;
        comma[0] = ',';
    }
    return s << ']';
}


#endif /* PIAOYIMQ_GENERAL_CPP_CODE_STL_CONTAINER_VECTOR_PRINT_VECTOR_H_ */
