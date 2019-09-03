//
// DOMWriter.cpp
//
// $Id: //poco/1.4/XML/samples/DOMWriter/src/DOMWriter.cpp#1 $
//
// This sample demonstrates the DOMWriter class and how to
// build DOM documents in memory.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//g++ DOMWriter.cpp -I /opt/poco_1_6/include/ -L /opt/poco_1_6/lib/ -lPocoXML -lPocoFoundation -Wl,-rpath,/opt/poco_1_6/lib/


#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/SAX/AttributesImpl.h"
//#include <cstdio>//std::perror
#include <cstring>
#include <cerrno>
#include <iostream>
#include <fstream>

namespace px = Poco::XML;
using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Text;
using Poco::XML::AutoPtr;
using Poco::XML::DOMWriter;
using Poco::XML::XMLWriter;


void testWriteXmlToStdout()
{
	// build a DOM document and write it to standard output//.

	AutoPtr<Document> pDoc = new Document;
	
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	pDoc->appendChild(pRoot);

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	AutoPtr<Text> pText1 = pDoc->createTextNode("text1");
	pChild1->appendChild(pText1);
	pRoot->appendChild(pChild1);

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	AutoPtr<Text> pText2 = pDoc->createTextNode("text2");
	pChild2->appendChild(pText2);
	pRoot->appendChild(pChild2);
	
	DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(XMLWriter::WRITE_XML_DECLARATION|XMLWriter::PRETTY_PRINT);

#if 0
	writer.writeNode(std::cout, pDoc);
#else
	std::ofstream xmlFile("test.xml");
	writer.writeNode(xmlFile, pDoc);
#endif

}


void testWriteXmlFile()
{

    std::ofstream str("test.xml");
    XMLWriter writer(str, XMLWriter::WRITE_XML_DECLARATION | XMLWriter::PRETTY_PRINT);
    writer.setNewLine("\n");
    writer.startDocument();
    Poco::XML::AttributesImpl attrs;
    attrs.addAttribute("", "", "a1", "", "v1");
    attrs.addAttribute("", "", "a2", "", "v2");
    writer.startElement("urn:mynamespace", "root", "", attrs);
    writer.startElement("", "", "sub");
    writer.endElement("", "", "sub");
    writer.endElement("urn:mynamespace", "root", "");
    writer.endDocument();
}


bool generateCrashXml()
{
    try
    {
        std::ofstream xmlFile("/opt/tmp/info.xml");
//        std::ofstream xmlFile("/tmp/crash-info.xml", std::ios::trunc);
        if(!xmlFile.good())
        {
//            std::perror("not good:");
            std::cout << "not good2:" << std::strerror(errno) << std::endl;
        }

        px::AutoPtr<px::Document> doc = new px::Document;

        px::AutoPtr<px::Element> crashInfo = doc->createElement("crash-info");
        doc->appendChild(crashInfo);

        px::AutoPtr<px::Element> timeZone = doc->createElement("time-zone");
        px::AutoPtr<px::Text> timeZoneValue = doc->createTextNode("...");
        crashInfo->appendChild(timeZone);
        timeZone->appendChild(timeZoneValue);

        px::AutoPtr<px::Element> crashTime = doc->createElement("crash-time");
        px::AutoPtr<px::Text> crashTimeValue = doc->createTextNode("...");
        crashInfo->appendChild(crashTime);
        crashTime->appendChild(crashTimeValue);

        px::DOMWriter writer;
        writer.setNewLine("\n");
        writer.setOptions(px::XMLWriter::WRITE_XML_DECLARATION | px::XMLWriter::PRETTY_PRINT);

        writer.writeNode(xmlFile, doc); //if open xmlFile failed, here will not throw exception
        std::cout << __LINE__<< std::endl;
//    } catch (const std::exception& ex)
    } catch(Poco::Exception)
    {
        std::cout << "Generate crash-info.xml exception: " << std::endl;
        return false;
    }

    std::cout << __LINE__<< std::endl;
    return true;
}

int main()
{
//    testWriteXmlToStdout();
//    testWriteXmlFile();
    generateCrashXml();
}
