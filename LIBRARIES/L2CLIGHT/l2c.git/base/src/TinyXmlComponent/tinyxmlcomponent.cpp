/*
    This file is part of LLCMc++ (a.k.a. llcmcpp)
    Copyright (C) 2011 INRIA Julien Bigot <julien.bigot@inria.fr>
    Copyright (C) 2011 INRIA Christian Pérez <christian.perez@inria.fr>

    LLCMc++ is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LLCMc++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LLCMc++.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include <llcmcpp3.h>

#include <tinyxml.h>

#include <xmlparser.h>

namespace llcmcpp {

namespace impl {

class TinyXmlComponent :
		virtual private XmlParser
{
public:
	TinyXmlComponent();
	
	virtual ~TinyXmlComponent();
	
	XmlParser* parser () { return this; }
	
	void visitor ( TiXmlVisitor* visitor ) { m_visitor = visitor; }
	
private:
	virtual void process_file ( const char* filename );
	
	virtual void process_data ( const char* xml );
	
	TiXmlVisitor* m_visitor;
	
};

}

}

using namespace llcmcpp;
using namespace llcmcpp::impl;
using namespace std;

TinyXmlComponent::TinyXmlComponent()
{
#ifndef NDEBUG
	std::cerr << "[TinyXmlComponent] Created" << endl;
#endif
}

TinyXmlComponent::~TinyXmlComponent()
{
#ifndef NDEBUG
  std::cerr << "[TinyXmlComponent] Destroyed" << endl;
#endif
}

void
TinyXmlComponent::process_data ( const char* xml )
{
#ifndef NDEBUG
	cerr << "[TinyXmlComponent] Parsing xml input" << endl;
#endif
	TiXmlDocument doc;
	const char* p=doc.Parse(xml);
	if (p) {
		doc.Accept(m_visitor);
	} else {
		cerr << "[TinyXmlComponent] ** Error when loading XML input: " << doc.ErrorDesc() << endl;
	}
	
}

void
TinyXmlComponent::process_file ( const char* filename )
{
#ifndef NDEBUG
	cerr << "[TinyXmlComponent] Loading file " << filename << endl;
#endif
	TiXmlDocument doc;
	bool r=doc.LoadFile(filename);
	if (r) {
		doc.Accept(m_visitor);
	} else {
		cerr << "[TinyXmlComponent] ** Error when loading XML file " << filename << ": " << doc.ErrorDesc() << " at " << doc.ErrorRow() << ":" << doc.ErrorCol() << endl;
	}
	
}

LCMP(TinyXmlComponent)
	L_CPP_USE(TiXmlVisitor, visitor)
	L_CPP_PROVIDE2(XmlParser, parser)
LEND
