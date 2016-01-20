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
#include <list>
#include <map>
#include <string>
#include <sstream>

#include <tinyxml.h>

#include <llcmcpp/clusterinstantiationmanagement.h>
#include <llcmcpp/corbamanagement.h>
#include <llcmcpp/cppmanagement2.h>
#include <llcmcpp/endpoint.h>
#include <llcmcpp/lifecycle.h>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/localinstantiationmanagement.h>
#include <llcmcpp/mpimanagement.h>
#include <llcmcpp/rmimanagement.h>
#include <llcmcpp/typesetter.h>
#include <llcmcpp/start.h>

#include <charminstantiationmanagement.h>

namespace gluon {

namespace impl {

using ::llcmcpp::ClusterInstantiationManagement;
using ::llcmcpp::EndPoint;
using ::llcmcpp::HostId;
using ::llcmcpp::CppManagement2;
using ::llcmcpp::RmiManagement;
using ::llcmcpp::Start;
using ::llcmcpp::TypeSetter;
using ::std::cerr;
using ::std::endl;
using ::std::istringstream;
using ::std::list;
using ::std::make_pair;
using ::std::map;
using ::std::multimap;
using ::std::pair;
using ::std::string;

class GadHandler :
		public TiXmlVisitor
{
public:
	RmiManagement* charm_management;
	
	RmiManagement* charm_array_management;
	
	RmiManagement* charm_group_management;
	
	CppManagement2* cpp_management;
	
	CharmInstantiationManagement* charm_instantiation_management;
	
	ClusterInstantiationManagement* instantiation_management;
	
	Start* start_management;
	
	TypeSetter* typed_configuration;
	
	GadHandler ():
			charm_management(0),
			charm_array_management(0),
			charm_group_management(0),
			cpp_management(0),
			charm_instantiation_management(0),
			instantiation_management(0),
			start_management(0),
			typed_configuration(0),
			m_current_host(-1),
			m_current_instance(0),
			m_current_port(0)
	{}
	
	virtual bool VisitEnter ( const TiXmlElement& elem, const TiXmlAttribute* attr )
	{
		// store namespaces
		for ( const TiXmlAttribute* cad_attr = attr; cad_attr != 0; cad_attr = cad_attr->Next() ) {
			string attr_name = string(cad_attr->Name());
			if ( attr_name.substr(0, 5) == "xmlns" && cad_attr->ValueStr() == "http://www.inria.fr/graal/gad/1.0.0") {
				if ( attr_name.size() > 5 ) m_namespaces.push_back(attr_name.substr(6)+":");
				else m_namespaces.push_back("");
			}
		}
		
		if ( is_node_type(elem, "gad") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] Document parsing started" << endl;
#endif
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "pe") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] next PE" << endl;
#endif
			++m_current_host;
			
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "start_property") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] start_property" << endl;
#endif
			start_management->start(endpoint(elem));
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "instance") ) {
			m_current_instance = required_attribute(elem, "id");
			string type = required_attribute(elem, "type");
			const char* from = attribute(elem, "from");
#ifndef NDEBUG
			cerr << "[GadHandler] instance " << m_current_instance << endl;
#endif
			if ( from ) instantiation_management->instantiate(m_current_host, m_current_instance, type, from);
			else instantiation_management->instantiate(m_current_host, m_current_instance, type);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "array") ) {
			m_current_instance = required_attribute(elem, "id");
			istringstream size_stream(required_attribute(elem, "size"));
			uint32_t size;
			size_stream >> size;
			string type = required_attribute(elem, "type");
			const char* from = attribute(elem, "from");
			const char* bound_to = attribute(elem, "bound_to");
#ifndef NDEBUG
			cerr << "[GadHandler] array "<<m_current_instance<<'['<<size<<']'<<endl;
#endif
			if ( bound_to ) {
#ifndef NDEBUG
				cerr << "[GadHandler] bound to "<<bound_to<<endl;
#endif
				if ( from ) charm_instantiation_management->instantiate_bound_array(m_current_instance, size, bound_to, type, from);
				else charm_instantiation_management->instantiate_bound_array(m_current_instance, size, bound_to, type);
			} else {
				if ( from ) charm_instantiation_management->instantiate_array(m_current_instance, size, type, from);
				else charm_instantiation_management->instantiate_array(m_current_instance, size, type);
			}
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "group") ) {
			m_current_instance = required_attribute(elem, "id");
			string type = required_attribute(elem, "type");
			const char* from = attribute(elem, "from");
#ifndef NDEBUG
			cerr << "[GadHandler] group " << m_current_instance << endl;
#endif
			if ( from ) charm_instantiation_management->instantiate_group(m_current_instance, type, from);
			else charm_instantiation_management->instantiate_group(m_current_instance, type);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "property") ) {
			m_current_port = required_attribute(elem, "id");
#ifndef NDEBUG
			cerr << "[GadHandler] property " << m_current_port << endl;
#endif
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "value") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] value" << endl;
#endif
			typed_configuration->configure(EndPoint(m_current_instance, m_current_port), required_attribute(elem, "type"), elem.GetText());
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "cppref") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] cppref" << endl;
#endif
			cpp_management->connect(
					EndPoint(m_current_instance, m_current_port),
					endpoint(elem)
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "charmref") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] charmref" << endl;
#endif
			charm_management->connect(
					EndPoint(m_current_instance, m_current_port),
					endpoint(elem)
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "arrayref") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] arrayref" << endl;
#endif
			charm_array_management->connect(
					EndPoint(m_current_instance, m_current_port),
					endpoint(elem)
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "groupref") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] groupref" << endl;
#endif
			charm_group_management->connect(
					EndPoint(m_current_instance, m_current_port),
					endpoint(elem)
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "systemref") ) {
#ifndef NDEBUG
			cerr << "[GadHandler] systemref" << endl;
#endif
			cpp_management->system_configure(
					EndPoint(m_current_instance, m_current_port),
					required_attribute(elem, "name")
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		const char* err_val = elem.Value();
		cerr << "[GadHandler] ** Error: Unknown element: " << elem.Value() << endl;
		abort();
		return false;
	}
	
private:
	list< string > m_namespaces;
	
	HostId m_current_host;
	
	const char* m_current_instance;
	
	const char* m_current_port;
	
	EndPoint endpoint ( const TiXmlElement& element )
	{
		return EndPoint(required_attribute(element, "instance"), required_attribute(element, "property"));
	}
	
	const char* attribute ( const TiXmlElement& element, const char* name )
	{
		const char* result = element.Attribute(name);
		for ( list< string >::iterator it = m_namespaces.begin(); it != m_namespaces.end() && !result; ++it ) {
			result = element.Attribute(((*it)+name).c_str());
		}
		return result;
	}
	
	const char* required_attribute ( const TiXmlElement& element, const char* name )
	{
		const char* result = attribute(element, name);
		if ( !result ) {
			cerr << "[GadHandler] XML Attribute '" << name << "' not found in node " << element.Value() << endl;
			abort();
		}
		return result;
	}
	
	bool is_node_type ( const TiXmlElement& element, string type_name )
	{
		string elem_name = element.ValueTStr();
		// test the no namespace case (we shouldn't but ...)
		if ( elem_name == type_name ) return true;
		// in the namespace case, the namespace is at least 1 char long as it contains the colon !
		if ( elem_name.length() <= type_name.length()+1 ) return false;
		// in the namespace case, the end should still match
		if ( elem_name.substr(elem_name.length()-type_name.length()) != type_name ) return false;
		string namespace_ = elem_name.substr(0, elem_name.length()-type_name.length());
		for ( list< string >::iterator nsit = m_namespaces.begin(); nsit != m_namespaces.end(); ++nsit ) {
			if ( namespace_ == *nsit ) return true;
		}
		return false;
	}
	
};

LCMP(GadHandler)
	L_CPP_PROVIDE(TiXmlVisitor, visitor);
	L_CPP_USE(RmiManagement, charm_management);
	L_CPP_USE(RmiManagement, charm_group_management);
	L_CPP_USE(RmiManagement, charm_array_management);
	L_CPP_USE(CppManagement2, cpp_management);
	L_CPP_USE(Start, start_management);
	L_CPP_USE(CharmInstantiationManagement, charm_instantiation_management);
	L_CPP_USE(ClusterInstantiationManagement, instantiation_management);
	L_CPP_USE(TypeSetter, typed_configuration);
LEND

}

}
