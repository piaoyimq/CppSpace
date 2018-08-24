/*
 * gzip-test.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: azhweib
 */




#include "Poco/DeflatingStream.h"
#include <fstream>
using Poco::DeflatingOutputStream;
using Poco::DeflatingStreamBuf;
int main(int argc, char** argv)
{
std::ofstream ostr("test.gz", std::ios::binary);
DeflatingOutputStream deflater(ostr, DeflatingStreamBuf::STREAM_GZIP);
deflater << "Hello, world!";
// ensure buffers get flushed before connected stream is closed
deflater.close();
ostr.close();
return 0;
}
