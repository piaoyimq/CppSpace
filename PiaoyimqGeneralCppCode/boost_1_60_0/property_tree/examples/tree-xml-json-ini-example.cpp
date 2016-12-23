/*
 * tree-xml-json-example.cpp
 *
 *  Created on: 2016年12月24日
 *      Author: piaoyimq
 */


#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <locale>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

int main(int argc, char **argv)
{
    /* The data format
     * <root>
     *  <num>1</num>
     *  <str>Test</str>
     * </root>
     */
    try
    {
        /* create the property tree */
        boost::property_tree::ptree datum;
        datum.put("root.num", 100);
        datum.put("root.str", "string");

        /* output XML string */
        std::ostringstream xmlOutputStream;
        boost::property_tree::xml_parser::write_xml(xmlOutputStream,
            datum);
        std::cout << "XML format:" << std::endl;
        std::cout << xmlOutputStream.str() << std::endl;

        /* output JSON string */
        std::ostringstream jsonOutputStream;
        boost::property_tree::json_parser::write_json(jsonOutputStream,
            datum);
        std::cout << "JSON format:" << std::endl;
        std::cout << jsonOutputStream.str() << std::endl;

        /* read datum from JSON stream */
        boost::property_tree::ptree ptParse;
        std::istringstream jsonIStream;
        jsonIStream.str(jsonOutputStream.str());
        boost::property_tree::json_parser::read_json(jsonIStream,
            ptParse);
        int num = ptParse.get<int>("root.num");
        std::string strVal = ptParse.get<std::string>("root.str");
        std::cout << "Num=" << std::dec << num
            << " Str=" << strVal << std::endl << std::endl;
    }
    catch (...)
    {
        printf("create boost::property_tree::ptree failed\n");
    }

    return 0;
}

