/*
 * gzip-test.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: azhweib
 */




#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace boost::iostreams;

template<typename T>
std::ostream &operator <<(std::ostream &os, const std::vector<T> &v) {
    using namespace std;
    copy(v.begin(), v.end(), ostream_iterator<T>(os));
    return os;
}

void test1()
{
    std::string OriginalString = "hello world";

    std::vector<char> CompressedVector;
    {
        filtering_ostream fos;    //??filter??????
        fos.push(gzip_compressor(gzip_params(gzip::best_compression)));  //gzip????
        fos.push(boost::iostreams::back_inserter(CompressedVector));     //??????????
        fos << OriginalString;
        boost::iostreams::close(fos);  //flush to CompressedVector ??????,CompressedVector????gzip??????
    }

    std::string decompressedString;
    {
        filtering_ostream fos;
        fos.push(gzip_decompressor());  //gzip?????
        fos.push(boost::iostreams::back_inserter(decompressedString)); //??????????
        fos.write(CompressedVector.data(), CompressedVector.size()); //?????????
        // ??????????????(ostream<<vector)???
        //out << compressedVector;
        boost::iostreams::close(fos); //flush. decompressedString?????????
    }

    if ( OriginalString == decompressedString) {
        std::cout << "test1() OK" << std::endl;
    }
}

void test2()
{
    std::string OriginalString = "hello world";

    std::string compressedString;
    {
        filtering_ostream fos;    //??filter??????
        fos.push(gzip_compressor(gzip_params(gzip::best_compression)));
        fos.push(boost::iostreams::back_inserter(compressedString));
        fos << OriginalString;   //?????????
        boost::iostreams::close(fos);  //flush. compressedString????????
                                       //??,????string???????,???????char??
    }

    std::string decompressedString;
    {
        filtering_ostream fos;
        fos.push(gzip_decompressor());
        fos.push(boost::iostreams::back_inserter(decompressedString));

        fos << compressedString; //???????????,????char=0???,???????????
                                 //strlen(compressedString.c_str() != compressedString.size())

        fos << std::flush;  //compressedString????????

    }

    if (OriginalString == decompressedString) {
        std::cout << "test2() OK" << std::endl;
    }
}

int main()
{
    test1();
    test2();
    system("pause");
}
