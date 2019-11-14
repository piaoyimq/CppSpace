/*
 * CodeFormatterExample.h
 *
 *  Created on: Nov 14, 2019
 *      Author: azhweib
 */


#ifndef TESTCELL_V93K_COMMON_CODEFORMATTEREXAMPLE_H_
#define TESTCELL_V93K_COMMON_CODEFORMATTEREXAMPLE_H_


#include <iostream>




class CodeFormatterExample
{
public:

    CodeFormatterExample();

    ~CodeFormatterExample();

    inline const std::string& getPrivateAttribute();

    void doComplexOperation();

    std::string publicAttribuate;


protected:

    void __doProtectedOperation();

    std::string __protectedAttribuate;


private:

    void _doPrivateOperation();

    std::string _privateAttribuate;
};


inline const std::string& CodeFormatterExample::getPrivateAttribute()
{
    return _privateAttribuate;
}


#endif /* TESTCELL_V93K_COMMON_CODEFORMATTEREXAMPLE_H_ */
