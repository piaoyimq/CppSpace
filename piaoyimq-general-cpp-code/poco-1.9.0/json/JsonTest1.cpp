/*
 * JsonTest1.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: azhweib
 */


//compile:  g++ --std=c++11 JsonTest1.cpp -I /opt/gss/poco_1_6/include/ -L /opt/gss/poco_1_6/lib/ -lPocoFoundation -lPocoJSON -Wl,-rpath,/opt/gss/poco_1_6/lib/

#include<iostream>
#include <stdint.h>
#include <sstream>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/StreamCopier.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/PrintHandler.h"


using namespace Poco::Dynamic;
using namespace Poco;
using std::string;

// json ??????
void JsonGet( )
{
#if 0
    string jsonString = "{ \"name\" : \"yuhaiyang\" }";
    JSON::Parser parser;
    Dynamic::Var result;
    parser.reset();

    result = parser.parse( jsonString );
    JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();
    Dynamic::Var ret = pObj->get( "name" );
    if ( ret.isEmpty() )
    {
        std::cout << "is null" << std::endl;
    }else
    {
        std::cout << ret.toString() << std::endl;
    }
#else
//    string jsonString = "[{ \"name\" : \"yuhaiyang\" }]";
    std::string jsonString("[{\"event\": \"ACTION_ASSIGNMENT_EVENT\", \"vname\": \"QQQ\"}, {\"event\": \"ACTION_ASSIGNMENT_EVENT\", \"vname\": \"FAMILY2\"}, {\"event\": \"LOT_STARTED\", \"vname\": \"STAGE\"}]");
    JSON::Parser parser;
    Dynamic::Var result;
    parser.reset();

    result = parser.parse( jsonString );
    JSON::Array::Ptr pArray = result.extract<JSON::Array::Ptr>();
    for (JSON::Array::ConstIterator it = pArray->begin() ; it != pArray->end(); it++ )
    {
//        std::cout << it->toString() << std::endl;
        parser.reset();
        result = parser.parse(it->toString());
        JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

        Dynamic::Var ret = pObj->get( "event" );
        if ( ret.isEmpty() )
        {
            std::cout << "is null" << std::endl;
        }else
        {
            std::cout << ret.toString() << std::endl;
        }

        ret = pObj->get( "vname" );
        if ( ret.isEmpty() )
        {
            std::cout << "is null" << std::endl;
        }else
        {
            std::cout << ret.toString() << std::endl;
        }
    }
#endif
}


void JsonArry( void )
{
    JSON::Object jsnObj;
    JSON::Array jsnArry;
    JSON::Object subObj1;
    JSON::Object subObj2;

    jsnObj.set( "command", "createuser" );

    subObj1.set( "name", "yuhaiyang");
    subObj1.set( "pass", "123" );

    subObj2.set( "name", "cj" );
    subObj2.set( "pass", "456" );

    jsnArry.add( subObj1 );
    jsnArry.add( subObj2 );

    jsnObj.set( "userinfo", jsnArry );

    std::stringstream  jsnString;
    jsnObj.stringify( jsnString, 3 );
    std::cout << jsnString.str() << std::endl;

}


void JsonGetArry( void )
{
    JSON::Object jsnObj;
    JSON::Array jsnArry;
    JSON::Object subObj1;
    JSON::Object subObj2;

    jsnObj.set( "command", "createuser" );

    subObj1.set( "name", "yuhaiyang");
    subObj1.set( "pass", "123" );

    subObj2.set( "name", "cj" );
    subObj2.set( "pass", "456" );

    jsnArry.add( subObj1 );
    jsnArry.add( subObj2 );

    jsnObj.set( "userinfo", jsnArry );

    std::stringstream  jsnOstream;
    jsnObj.stringify( jsnOstream, 3 );

    string jsnStr = jsnOstream.str();

    std::cout << "???:\n" << jsnStr << "\n\n\n" << std::endl;


    //--------------------------????-----------------------------
    JSON::Parser parse;
    Dynamic::Var json = parse.parse( jsnStr );
    JSON::Object::Ptr pObj = json.extract<JSON::Object::Ptr>();
    JSON::Array::Ptr pArry = pObj->getArray( "userinfo" );

    JSON::Array::ConstIterator it = pArry->begin();
    //?????????????
    //???????????????
    for ( ; it != pArry->end(); it++ )
    {
        std::cout << it->toString() << std::endl;
    }

}

void getJsonArry(const JSON::Object::Ptr& ptr, const std::string& keyName, std::vector<std::string>& keyVec)
{
    JSON::Array::Ptr arry = ptr->getArray(keyName);

    JSON::Array::ConstIterator it;

    JSON::Parser parser;
    Dynamic::Var result;

    for (it = arry->begin(); it != arry->end(); it++)
    {
//        std::cout << it->toString() << std::endl;

        parser.reset();

        result = parser.parse(it->toString());
        JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

        Dynamic::Var ret = pObj->get("v");

        if (ret.isEmpty())
        {
            std::cout << "is null" << std::endl;
        }
        else
        {
            if ("1" == ret.toString())
            {
//                std::cout << ret.toString() << std::endl;
//                std::cout << pObj->get( "key" ).toString() << std::endl;
                keyVec.push_back(pObj->get("key").toString());
            }
        }
    }
}


void testJsonParse()
{

    std::string jsonString("{\"sys\": [{\"v\": \"1\", \"id\": \"k1\", \"key\": \"receiving SRQ from prober timed out\"}, {\"v\": \"1\", \"id\": \"k2\", \"key\": \"waiting for operation to complete, heartbeat timeout\"}, {\"v\": \"1\", \"id\": \"k3\", \"key\": \"waiting_for_lot_timeout\"}, {\"v\": \"0\", \"id\": \"k4\", \"key\": \"general_timeout\"}, {\"v\": \"0\", \"id\": \"k5\", \"key\": \"ERROR\"}, {\"v\": \"0\", \"id\": \"k6\", \"key\": \"Error\"}, {\"v\": \"0\", \"id\": \"k7\", \"key\": \"error\"}, {\"v\": \"0\", \"id\": \"k8\", \"key\": \"Not Calibrated\"}], \"user\": [{\"v\": \"0\", \"key\": \"teseterddd\"}]}");
    JSON::Parser parse;
    Dynamic::Var json = parse.parse( jsonString );
    JSON::Object::Ptr pObj = json.extract<JSON::Object::Ptr>();
#if 0
    JSON::Array::Ptr sysArry = pObj->getArray( "sys" );

    JSON::Array::ConstIterator it = sysArry->begin();

    JSON::Parser parser;
    Dynamic::Var result;

    for ( ; it != sysArry->end(); it++ )
    {
        std::cout << it->toString() << std::endl;

        parser.reset();

        result = parser.parse( it->toString() );
        JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

        Dynamic::Var ret = pObj->get( "v" );

        if ( ret.isEmpty() )
        {
            std::cout << "is null" << std::endl;
        }else
        {
            if("1" == ret.toString())
            {
                std::cout << ret.toString() << std::endl;
                std::cout << pObj->get( "key" ).toString() << std::endl;
            }

        }
    }
#else
    std::vector<std::string> keyVec;
    getJsonArry(pObj, "sys", keyVec);
    getJsonArry(pObj, "user", keyVec);
    for(auto& it:keyVec)
    {
        std::cout << it << std::endl;
    }

#endif

}


int main()
{
    JsonGet();
    JsonArry();
    JsonGetArry();
    testJsonParse();
}

