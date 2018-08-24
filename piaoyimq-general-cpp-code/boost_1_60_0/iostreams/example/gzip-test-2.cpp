/*
 * gzip-test-2.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: azhweib
 */




#include<iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
using namespace boost;
using namespace std;

bool compress()
{
        std::vector<char> content;

        std::ifstream file;
        file.open("/home/jk/hello",std::ios::binary);
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0);

        content.resize(size);
        file.read(&(content.front()), size);
        file.close();

        std::vector<char> compressed;
        boost::iostreams::filtering_streambuf<boost::iostreams::output> compress_out;
        compress_out.push(boost::iostreams::zlib_compressor());
        compress_out.push(boost::iostreams::back_inserter(compressed));

        boost::iostreams::copy(boost::make_iterator_range(content), compress_out);

        std::ofstream save;
        save.open("/home/jk/hello.zip",std::ios::binary);
        save.write(&(compressed.front()), compressed.size());
        save.close();

        return true;
}

int main()
{
  compress();
  return 0x00;
}
