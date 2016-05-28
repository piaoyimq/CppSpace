/*
 * test.cpp
 *
 *  Created on: 2016年5月28日
 *      Author: piaoyimq
 */
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
using namespace std;

class Data
{
  friend class boost::serialization::access;
  int64_t m_data;
  template <class Archive>

  void serialize( Archive& ar, const unsigned int version )
  {
    ar & m_data;
  };

public:
  Data() : m_data( 0 ) {};
  const int64_t& getData() const { return m_data; };
  void setData( int64_t val ){ m_data = val; }
};



int main(int argc,  char* argv[])
{
    // 序列化对象
    ostringstream oss;
    boost::archive::binary_oarchive oar( oss );
    Data data;
    const Data& data_freeze = const_cast<Data&>( data );
    data.setData( 1000000000000000000 );
    oar << data_freeze;
    data.setData( 4000000000000000000 );
    oar << data_freeze;
    cout << "serialize: len:" << oss.str().size() << "\ndata:" << oss.str() << endl;

    cout << "\n__________________\n" << endl;
    // 反序列化
    istringstream iss( oss.str() );
    boost::archive::binary_iarchive iar( iss );
    Data data1;
    iar >> data1;
    cout << "data: " << data1.getData() << endl;
    iar >> data1;
    cout << "data: " << data1.getData() << endl;
//    getchar();
    return 0;
}


