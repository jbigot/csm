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

#include <tinyxml.h>

#include <llcmcpp/corbamanagement.h>
#include <llcmcpp/cppmanagement.h>
#include <llcmcpp/distributedinstantiationmanagement.h>
#include <llcmcpp/endpoint.h>
#include <llcmcpp/gridinstantiationmanagement.h>
#include <llcmcpp/lifecycle.h>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/mpimanagement.h>
#include <llcmcpp/rmimanagement.h>
#include <llcmcpp/typesetter.h>

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::make_pair;
using ::std::map;
using ::std::multimap;
using ::std::pair;
using ::std::string;

namespace llcmcpp {

namespace impl {

class LadHandler :
		public TiXmlVisitor
{
public:
	RmiManagement* charm_management;
	
	RmiManagement* corba_management;
	
	CppManagement* cpp_management;
	
	DistributedInstantiationManagement* distributed_instantiation_management;
	
	GridInstantiationManagement* grid_instantiation_management;
	
	MpiManagement* mpi_management;
	
	TypeSetter* typed_configuration;
	
	LadHandler ():
			charm_management(0),
			corba_management(0),
			cpp_management(0),
			distributed_instantiation_management(0),
			grid_instantiation_management(0),
			mpi_management(0),
			typed_configuration(0),
			m_in_mpi(false),
			m_mpi_rank(-1),
			m_current_instance(0),
			m_current_port(0)
	{}
	
	virtual bool VisitEnter ( const TiXmlElement& elem, const TiXmlAttribute* attr )
	{
		// store namespaces
		for ( const TiXmlAttribute* cad_attr = attr; cad_attr != 0; cad_attr = cad_attr->Next() ) {
			string attr_name = string(cad_attr->Name());
			if ( attr_name.substr(0, 5) == "xmlns" && cad_attr->ValueStr() == "http://www.inria.fr/graal/lad/1.0.0") {
				if ( attr_name.size() > 5 ) m_namespaces.push_back(attr_name.substr(6)+":");
				else m_namespaces.push_back("");
			}
		}
		
		if ( is_node_type(elem, "lad") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] Document parsing started" << endl;
#endif
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "mpi") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] mpi" << endl;
#endif
			m_in_mpi = true;
			m_mpi_rank = -1;
			m_current_host = attribute(elem, "id");
			if ( ! m_current_host ) m_current_host = "";
#ifndef NDEBUG
			if (m_current_host) cerr << "[LadHandler] current_process=("<<m_current_host<<", "<<m_mpi_rank<<')'<<endl;
			else cerr << "[LadHandler] current_process=("<<m_mpi_rank<<')'<<endl;
#endif
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "communicator") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] communicator" << endl;
#endif
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "peer") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] peer" << endl;
#endif
			m_current_communicator.push_back(endpoint(elem));
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "process") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] process"<<endl;
#endif
			if ( m_in_mpi ) ++m_mpi_rank;
			else m_current_host = attribute(elem, "id");
			
			if ( !m_current_host ) m_current_host = "";
			
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "start_property") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] start_property" << endl;
#endif
			cpp_management->start(endpoint(elem));
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "instance") ) {
			m_current_instance = required_attribute(elem, "id");
			string type = required_attribute(elem, "type");
			const char* from = attribute(elem, "from");
#ifndef NDEBUG
			cerr << "[LadHandler] instance " << m_current_instance << endl;
#endif
			if (!m_in_mpi)
			  if ( from ) distributed_instantiation_management->instantiate(m_current_host, m_current_instance, type, from);
			  else distributed_instantiation_management->instantiate(m_current_host, m_current_instance, type);
			else
			  if ( from ) grid_instantiation_management->instantiate(m_current_host, m_mpi_rank, m_current_instance, type, from);
			  else grid_instantiation_management->instantiate(m_current_host, m_mpi_rank, m_current_instance, type);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "property") ) {
			m_current_port = required_attribute(elem, "id");
#ifndef NDEBUG
			cerr << "[LadHandler] property " << m_current_port << endl;
#endif
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "value") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] value" << endl;
#endif
			typed_configuration->configure(EndPoint(m_current_instance, m_current_port), required_attribute(elem, "type"), elem.GetText());
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "cppref") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] cppref" << endl;
#endif
			cpp_management->connect(
					EndPoint(m_current_instance, m_current_port),
					endpoint(elem)
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "corbaref") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] corbaref" << endl;
#endif
			corba_management->connect(
					EndPoint(m_current_instance, m_current_port),
					endpoint(elem)
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		if ( is_node_type(elem, "systemref") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] systemref" << endl;
#endif
			cpp_management->system_configure(
					EndPoint(m_current_instance, m_current_port),
					required_attribute(elem, "name")
			);
			return TiXmlVisitor::VisitEnter(elem, attr);
		}
		
		cerr << "[LadHandler] ** Error: Unknown element: " << elem.Value() << endl;
		abort();
		return false;
	}
	
    virtual bool VisitExit ( const TiXmlElement& elem )
    {
		if ( is_node_type(elem, "communicator") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] /communicator" << endl;
#endif
			mpi_management->mpi_connect(m_current_communicator);
			m_current_communicator.clear();
			return TiXmlVisitor::VisitExit(elem);
		}
		if ( is_node_type(elem, "mpi") ) {
#ifndef NDEBUG
			cerr << "[LadHandler] /mpi" << endl;
#endif
			m_in_mpi = false;
		}
		return TiXmlVisitor::VisitExit(elem);
	}
	
private:
	list< string > m_namespaces;
	
	list< EndPoint > m_current_communicator;
	
	bool m_in_mpi;
	
	int m_mpi_rank;
	
	const char* m_current_host;
	
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
			cerr << "[LadHandler] XML Attribute '" << name << "' not found in node " << element.Value() << endl;
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

LCMP(LadHandler)
	L_CPP_PROVIDE(TiXmlVisitor, visitor);
	L_CPP_USE(RmiManagement, charm_management);
	L_CPP_USE(RmiManagement, corba_management);
	L_CPP_USE(CppManagement, cpp_management);
	L_CPP_USE(DistributedInstantiationManagement, distributed_instantiation_management);
	L_CPP_USE(GridInstantiationManagement, grid_instantiation_management);
	L_CPP_USE(MpiManagement, mpi_management);
	L_CPP_USE(TypeSetter, typed_configuration);
LEND

}

}


