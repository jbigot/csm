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
    along with LLCMc++.  If not, see <http://www.gnu.org/licenses));.
*/

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>      // std::stringstream

#include </home/hcoullon/Documents/Libraries/L2C_OpenMPI/l2c.git/tinyxml/tinyxml.h>

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
#include <llcmcpp/xmlparser.h>

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

class Assembly :
		public XmlParser
{
public:
	
	CppManagement* cpp_management;
	
	GridInstantiationManagement* grid_instantiation_management;
	
	MpiManagement* mpi_management;
	
	TypeSetter* typed_configuration;
	
	Assembly ():
			cpp_management(0),
			grid_instantiation_management(0),
			mpi_management(0),
			typed_configuration(0)
	{}

	void create_assembly(const char * width,const char * height,const char* it,const char * proc)
	{
		int p;
		std::stringstream pss(proc);
		pss >> p;

		std::list<EndPoint> comm;

		for(int i=0;i<p;i++)
		{
			string istr;
			std::stringstream pss2;
			pss2 << i;
			istr = pss2.str();

			//start
			cpp_management->start(EndPoint(string("dr_")+istr,"inGo"));
			
			//driver
			grid_instantiation_management->instantiate("", 0,string("dr_")+istr, "Driver");
			cpp_management->connect(EndPoint(string("dr_")+istr,"structInit"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("dr_")+istr,"driverappGo"),EndPoint(string("dra_")+istr,"inGo"));

			//driver app
			grid_instantiation_management->instantiate("", 0,string("dra_")+istr, "DriverApp");
			cpp_management->connect(EndPoint(string("dra_")+istr,"timeGo"),EndPoint(string("ti_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("u_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("v_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("z_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("hs_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("us_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("vs_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("hsa_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("usa_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("vsa_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("q1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("q2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("qs1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("qs2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("f1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("f2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("f3_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("g1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("g2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("g3_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h1d_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h1g_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h1l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h1r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h2d_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h2g_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h2l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("h2r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("delz1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("delz2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("delzc1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("delzc2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("qsa1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("qsa2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("u1l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("u1r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("u2l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("u2r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("v1l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("v1r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("v2l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("v2r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("z1l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("z1r_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("z2l_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("dra_")+istr,"dataCompute"),EndPoint(string("z2r_")+istr,"inGo"));

			//Structure
			grid_instantiation_management->instantiate("", 0,string("st_")+istr, "StructureSK");
			typed_configuration->configure(EndPoint(string("st_")+istr,"globW"),"uint64",width);
			typed_configuration->configure(EndPoint(string("st_")+istr,"globH"),"uint64",height);
			typed_configuration->configure(EndPoint(string("st_")+istr,"nb_proc"),"uint32",proc);

			//Data
			grid_instantiation_management->instantiate("", 0,string("h_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("h_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("h_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("h_")+istr,"init_val"),"double","1.0");
			
			grid_instantiation_management->instantiate("", 0,string("u_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("u_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("u_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("u_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("u_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("u_")+istr,"init_val"),"double","1.0");
			
			grid_instantiation_management->instantiate("", 0,string("v_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("v_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("v_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("v_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("v_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("v_")+istr,"init_val"),"double","1.0");
			
			grid_instantiation_management->instantiate("", 0,string("z_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("z_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("z_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("z_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("z_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("z_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("hs_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("hs_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("hs_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("hs_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("hs_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("hs_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("us_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("us_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("us_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("us_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("us_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("us_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("vs_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("vs_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("vs_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("vs_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("vs_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("vs_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("hsa_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("hsa_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("hsa_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("hsa_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("hsa_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("hsa_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("usa_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("usa_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("usa_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("usa_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("usa_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("usa_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("vsa_")+istr,"DataSK");
			cpp_management->connect(EndPoint(string("vsa_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("vsa_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("vsa_")+istr,"border"),"uint16","2");
			typed_configuration->configure(EndPoint(string("vsa_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("vsa_")+istr,"init_val"),"double","1.0");

			grid_instantiation_management->instantiate("", 0,string("q1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("q1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("q1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("q1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("q1_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("q1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("q2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("q2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("q2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("q2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("q2_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("q2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("qs1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("qs1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("qs1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("qs1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("qs1_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("qs1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("qs2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("qs2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("qs2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("qs2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("qs2_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("qs2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("f1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("f1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("f1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("f1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("f1_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("f1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("f2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("f2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("f2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("f2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("f2_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("f2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("f3_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("f3_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("f3_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("f3_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("f3_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("f3_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("g1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("g1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("g1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("g1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("g1_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("g1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("g2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("g2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("g2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("g2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("g2_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("g2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("g3_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("g3_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("g3_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("g3_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("g3_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("g3_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h1d_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h1d_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h1d_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h1d_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h1d_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("h1d_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h1g_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h1g_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h1g_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h1g_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h1g_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("h1g_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h1l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h1l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h1l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h1l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h1l_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("h1l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h1r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h1r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h1r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h1r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h1r_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("h1r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h2d_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h2d_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h2d_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h2d_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h2d_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("h2d_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h2g_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h2g_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h2g_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h2g_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h2g_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("h2g_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h2l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h2l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h2l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h2l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h2l_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("h2l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("h2r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("h2r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("h2r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("h2r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("h2r_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("h2r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("delz1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("delz1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delz1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("delz1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("delz1_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("delz1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("delz2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("delz2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delz2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("delz2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("delz2_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("delz2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("delzc1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("delzc1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("delzc1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("delzc1_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("delzc1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("delzc2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("delzc2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("delzc2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("delzc2_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("delzc2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("qsa1_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("qsa1_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("qsa1_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("qsa1_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("qsa1_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("qsa1_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("qsa2_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("qsa2_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("qsa2_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("qsa2_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("qsa2_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("qsa2_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("u1l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("u1l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("u1l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("u1l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("u1l_")+istr,"domain"),"uint16","1");
			typed_configuration->configure(EndPoint(string("u1l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("u1r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("u1r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("u1r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("u1r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("u1r_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("u1r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("u2l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("u2l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("u2l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("u2l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("u2l_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("u2l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("u2r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("u2r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("u2r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("u2r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("u2r_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("u2r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("v1l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("v1l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("v1l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("v1l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("v1l_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("v1l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("v1r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("v1r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("v1r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("v1r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("v1r_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("v1r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("v2l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("v2l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("v2l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("v2l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("v2l_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("v2l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("v2r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("v2r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("v2r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("v2r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("v2r_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("v2r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("z1l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("z1l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("z1l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("z1l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("z1l_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("z1l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("z1r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("z1r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("z1r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("z1r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("z1r_")+istr,"domain"),"uint16","2");
			typed_configuration->configure(EndPoint(string("z1r_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("z2l_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("z2l_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("z2l_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("z2l_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("z2l_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("z2l_")+istr,"init_val"),"double","0.0");

			grid_instantiation_management->instantiate("", 0,string("z2r_")+istr,"DataSK0");
			cpp_management->connect(EndPoint(string("z2r_")+istr,"str"),EndPoint(string("st_")+istr,"services"));
			cpp_management->connect(EndPoint(string("z2r_")+istr,"initializer"),EndPoint(string("in_")+istr,"initdata"));
			typed_configuration->configure(EndPoint(string("z2r_")+istr,"border"),"uint16","0");
			typed_configuration->configure(EndPoint(string("z2r_")+istr,"domain"),"uint16","3");
			typed_configuration->configure(EndPoint(string("z2r_")+istr,"init_val"),"double","0.0");

			//Time
			grid_instantiation_management->instantiate("", 0,string("in_")+istr,"Initializer");
			grid_instantiation_management->instantiate("", 0,string("ti_")+istr,"Time");
			typed_configuration->configure(EndPoint(string("ti_")+istr,"iter"),"uint16",it);
			cpp_management->connect(EndPoint(string("ti_")+istr,"phaseCompute"),EndPoint(string("Seq0_")+istr,"inGo"));

			//sequence
			grid_instantiation_management->instantiate("", 0,string("Seq0_")+istr,"Sequence");
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhd_limud_limvd_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhl_limul_limvl_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhr_limur_limvr_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhu_limuu_limvu_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("Sync0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("delzf_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("delzc1f_delzc2f_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("delzf_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1x1_bloc1x2_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1y1_bloc1y2_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhd_limud_limvd_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhl_limul_limvl_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhr_limur_limvr_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("limhu_limuu_limvu_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("Sync1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("delzf_2_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("delzc1f_delzc2f_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("delzf_3_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1x1_bloc1x2_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1y1_bloc1y2_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"inGo"));
			cpp_management->connect(EndPoint(string("Seq0_")+istr,"Compute"),EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"inGo"));

			//kernels
			grid_instantiation_management->instantiate("", 0,string("limhl_limul_limvl_0_")+istr,"Klimhl_limul_limvl");
			cpp_management->connect(EndPoint(string("limhl_limul_limvl_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhl_limul_limvl_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhl_limul_limvl_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhr_limur_limvr_0_")+istr,"Klimhr_limur_limvr");
			cpp_management->connect(EndPoint(string("limhr_limur_limvr_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhr_limur_limvr_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhr_limur_limvr_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhu_limuu_limvu_0_")+istr,"Klimhu_limuu_limvu");
			cpp_management->connect(EndPoint(string("limhu_limuu_limvu_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhu_limuu_limvu_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhu_limuu_limvu_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhd_limud_limvd_0_")+istr,"Klimhd_limud_limvd");
			cpp_management->connect(EndPoint(string("limhd_limud_limvd_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhd_limud_limvd_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhd_limud_limvd_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("Sync0_")+istr,"Sync");
			cpp_management->connect(EndPoint(string("Sync0_")+istr,"dataCompute"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("Sync0_")+istr,"dataCompute"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("Sync0_")+istr,"dataCompute"),EndPoint(string("v_")+istr,"services"));
			cpp_management->connect(EndPoint(string("Sync0_")+istr,"dataCompute"),EndPoint(string("z_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r");
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"z"),EndPoint(string("z_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"h1l"),EndPoint(string("h1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"h1r"),EndPoint(string("h1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"u1l"),EndPoint(string("u1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"u1r"),EndPoint(string("u1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"v1l"),EndPoint(string("v1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"v1r"),EndPoint(string("v1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"z1l"),EndPoint(string("z1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_0_")+istr,"z1r"),EndPoint(string("z1r_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r");
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"z"),EndPoint(string("z_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"h2l"),EndPoint(string("h2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"h2r"),EndPoint(string("h2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"u2l"),EndPoint(string("u2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"u2r"),EndPoint(string("u2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"v2l"),EndPoint(string("v2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"v2r"),EndPoint(string("v2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"z2l"),EndPoint(string("z2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_0_")+istr,"z2r"),EndPoint(string("z2r_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("delzf_0_")+istr,"Kdelzf");
			cpp_management->connect(EndPoint(string("delzf_0_")+istr,"z1l"),EndPoint(string("z1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_0_")+istr,"z1r"),EndPoint(string("z1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_0_")+istr,"delz1"),EndPoint(string("delz1_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("delzc1f_delzc2f_0_")+istr,"Kdelzc1f_delzc2f");
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_0_")+istr,"z1l"),EndPoint(string("z1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_0_")+istr,"z1r"),EndPoint(string("z1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_0_")+istr,"z2l"),EndPoint(string("z2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_0_")+istr,"z2r"),EndPoint(string("z2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_0_")+istr,"delzc1"),EndPoint(string("delzc1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_0_")+istr,"delzc2"),EndPoint(string("delzc2_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("delzf_1_")+istr,"Kdelzf");
			cpp_management->connect(EndPoint(string("delzf_1_")+istr,"z1l"),EndPoint(string("z2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_1_")+istr,"z1r"),EndPoint(string("z2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_1_")+istr,"delz1"),EndPoint(string("delz2_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1x1_bloc1x2_0_")+istr,"Kbloc1x1_bloc1x2");
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_0_")+istr,"h1l"),EndPoint(string("h1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_0_")+istr,"delz1"),EndPoint(string("delz1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_0_")+istr,"h1r"),EndPoint(string("h1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_0_")+istr,"h1g"),EndPoint(string("h1g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_0_")+istr,"h1d"),EndPoint(string("h1d_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1y1_bloc1y2_0_")+istr,"Kbloc1y1_bloc1y2");
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_0_")+istr,"h2l"),EndPoint(string("h2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_0_")+istr,"delz2"),EndPoint(string("delz2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_0_")+istr,"h2r"),EndPoint(string("h2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_0_")+istr,"h2g"),EndPoint(string("h2g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_0_")+istr,"h2d"),EndPoint(string("h2d_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"Kbloc1x3_bloc1x4_bloc1x5");
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"u1l"),EndPoint(string("u1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"u1r"),EndPoint(string("u1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"h1g"),EndPoint(string("h1g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"h1d"),EndPoint(string("h1d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"v1l"),EndPoint(string("v1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"v1r"),EndPoint(string("v1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"f1"),EndPoint(string("f1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"f2"),EndPoint(string("f2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_0_")+istr,"f3"),EndPoint(string("f3_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"Kbloc1y3_bloc1y4_bloc1y5");
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"u2l"),EndPoint(string("u2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"u2r"),EndPoint(string("u2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"h2g"),EndPoint(string("h2g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"h2d"),EndPoint(string("h2d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"v2l"),EndPoint(string("v2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"v2r"),EndPoint(string("v2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"g1"),EndPoint(string("g1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"g2"),EndPoint(string("g2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_0_")+istr,"g3"),EndPoint(string("g3_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"Kbloc21_bloc22_bloc23_check_checkus_checkvs");
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"f1"),EndPoint(string("f1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"g1"),EndPoint(string("g1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"u"),EndPoint(string("u_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"v"),EndPoint(string("v_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"q1"),EndPoint(string("q1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h1l"),EndPoint(string("h1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h1r"),EndPoint(string("h1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h1g"),EndPoint(string("h1g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h1d"),EndPoint(string("h1d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"delzc1"),EndPoint(string("delzc1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"f2"),EndPoint(string("f2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"g2"),EndPoint(string("g2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"hs"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"q2"),EndPoint(string("q2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h2l"),EndPoint(string("h2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h2r"),EndPoint(string("h2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h2d"),EndPoint(string("h2d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"h2g"),EndPoint(string("h2g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"delzc2"),EndPoint(string("delzc2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"f3"),EndPoint(string("f3_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"g3"),EndPoint(string("g3_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"us"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"qs1"),EndPoint(string("qs1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"vs"),EndPoint(string("vs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_check_checkus_checkvs_0_")+istr,"qs2"),EndPoint(string("qs2_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhl_limul_limvl_1_")+istr,"Klimhl_limul_limvl");
			cpp_management->connect(EndPoint(string("limhl_limul_limvl_1_")+istr,"h"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhl_limul_limvl_1_")+istr,"u"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhl_limul_limvl_1_")+istr,"v"),EndPoint(string("vs_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhr_limur_limvr_1_")+istr,"Klimhr_limur_limvr");
			cpp_management->connect(EndPoint(string("limhr_limur_limvr_1_")+istr,"h"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhr_limur_limvr_1_")+istr,"u"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhr_limur_limvr_1_")+istr,"v"),EndPoint(string("vs_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhu_limuu_limvu_1_")+istr,"Klimhu_limuu_limvu");
			cpp_management->connect(EndPoint(string("limhu_limuu_limvu_1_")+istr,"h"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhu_limuu_limvu_1_")+istr,"u"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhu_limuu_limvu_1_")+istr,"v"),EndPoint(string("vs_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("limhd_limud_limvd_1_")+istr,"Klimhd_limud_limvd");
			cpp_management->connect(EndPoint(string("limhd_limud_limvd_1_")+istr,"h"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhd_limud_limvd_1_")+istr,"u"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("limhd_limud_limvd_1_")+istr,"v"),EndPoint(string("vs_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("Sync1_")+istr,"Sync");
			cpp_management->connect(EndPoint(string("Sync1_")+istr,"dataCompute"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("Sync1_")+istr,"dataCompute"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("Sync1_")+istr,"dataCompute"),EndPoint(string("vs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("Sync1_")+istr,"dataCompute"),EndPoint(string("z_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r");
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"h"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"u"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"v"),EndPoint(string("vs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"z"),EndPoint(string("z_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"h1l"),EndPoint(string("h1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"h1r"),EndPoint(string("h1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"u1l"),EndPoint(string("u1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"u1r"),EndPoint(string("u1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"v1l"),EndPoint(string("v1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"v1r"),EndPoint(string("v1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"z1l"),EndPoint(string("z1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_1_")+istr,"z1r"),EndPoint(string("z1r_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r");
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"h"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"u"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"v"),EndPoint(string("vs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"z"),EndPoint(string("z_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"h2l"),EndPoint(string("h2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"h2r"),EndPoint(string("h2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"u2l"),EndPoint(string("u2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"u2r"),EndPoint(string("u2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"v2l"),EndPoint(string("v2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"v2r"),EndPoint(string("v2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"z2l"),EndPoint(string("z2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("huvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r_1_")+istr,"z2r"),EndPoint(string("z2r_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("delzf_2_")+istr,"Kdelzf");
			cpp_management->connect(EndPoint(string("delzf_2_")+istr,"z1l"),EndPoint(string("z1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_2_")+istr,"z1r"),EndPoint(string("z1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_2_")+istr,"delz1"),EndPoint(string("delz1_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("delzc1f_delzc2f_1_")+istr,"Kdelzc1f_delzc2f");
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_1_")+istr,"z1l"),EndPoint(string("z1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_1_")+istr,"z1r"),EndPoint(string("z1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_1_")+istr,"z2l"),EndPoint(string("z2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_1_")+istr,"z2r"),EndPoint(string("z2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_1_")+istr,"delzc1"),EndPoint(string("delzc1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzc1f_delzc2f_1_")+istr,"delzc2"),EndPoint(string("delzc2_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("delzf_3_")+istr,"Kdelzf");
			cpp_management->connect(EndPoint(string("delzf_3_")+istr,"z1l"),EndPoint(string("z2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_3_")+istr,"z1r"),EndPoint(string("z2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("delzf_3_")+istr,"delz1"),EndPoint(string("delz2_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1x1_bloc1x2_1_")+istr,"Kbloc1x1_bloc1x2");
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_1_")+istr,"h1l"),EndPoint(string("h1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_1_")+istr,"delz1"),EndPoint(string("delz1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_1_")+istr,"h1r"),EndPoint(string("h1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_1_")+istr,"h1g"),EndPoint(string("h1g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x1_bloc1x2_1_")+istr,"h1d"),EndPoint(string("h1d_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1y1_bloc1y2_1_")+istr,"Kbloc1y1_bloc1y2");
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_1_")+istr,"h2l"),EndPoint(string("h2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_1_")+istr,"delz2"),EndPoint(string("delz2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_1_")+istr,"h2r"),EndPoint(string("h2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_1_")+istr,"h2g"),EndPoint(string("h2g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y1_bloc1y2_1_")+istr,"h2d"),EndPoint(string("h2d_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"Kbloc1x3_bloc1x4_bloc1x5");
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"u1l"),EndPoint(string("u1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"u1r"),EndPoint(string("u1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"h1g"),EndPoint(string("h1g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"h1d"),EndPoint(string("h1d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"v1l"),EndPoint(string("v1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"v1r"),EndPoint(string("v1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"f1"),EndPoint(string("f1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"f2"),EndPoint(string("f2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1x3_bloc1x4_bloc1x5_1_")+istr,"f3"),EndPoint(string("f3_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"Kbloc1y3_bloc1y4_bloc1y5");
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"u2l"),EndPoint(string("u2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"u2r"),EndPoint(string("u2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"h2g"),EndPoint(string("h2g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"h2d"),EndPoint(string("h2d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"v2l"),EndPoint(string("v2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"v2r"),EndPoint(string("v2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"g1"),EndPoint(string("g1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"g2"),EndPoint(string("g2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc1y3_bloc1y4_bloc1y5_1_")+istr,"g3"),EndPoint(string("g3_")+istr,"services"));

			grid_instantiation_management->instantiate("", 0,string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"Kbloc21_bloc22_bloc23_heun1_heun2_heun3");
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h"),EndPoint(string("h_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"hs"),EndPoint(string("hs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"f1"),EndPoint(string("f1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"g1"),EndPoint(string("g1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"us"),EndPoint(string("us_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"vs"),EndPoint(string("vs_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"qs1"),EndPoint(string("qs1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h1l"),EndPoint(string("h1l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h1r"),EndPoint(string("h1r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h1g"),EndPoint(string("h1g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h1d"),EndPoint(string("h1d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"delzc1"),EndPoint(string("delzc1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"f2"),EndPoint(string("f2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"g2"),EndPoint(string("g2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"hsa"),EndPoint(string("hsa_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"qs2"),EndPoint(string("qs2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h2l"),EndPoint(string("h2l_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h2r"),EndPoint(string("h2r_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h2d"),EndPoint(string("h2d_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"h2g"),EndPoint(string("h2g_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"delzc2"),EndPoint(string("delzc2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"f3"),EndPoint(string("f3_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"g3"),EndPoint(string("g3_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"qsa1"),EndPoint(string("qsa1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"qsa2"),EndPoint(string("qsa2_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"q1"),EndPoint(string("q1_")+istr,"services"));
			cpp_management->connect(EndPoint(string("bloc21_bloc22_bloc23_heun1_heun2_heun3_0_")+istr,"q2"),EndPoint(string("q2_")+istr,"services"));

			comm.push_back(EndPoint(string("h_")+istr,"comm"));
			comm.push_back(EndPoint(string("u_")+istr,"comm"));
			comm.push_back(EndPoint(string("v_")+istr,"comm"));
			comm.push_back(EndPoint(string("hs_")+istr,"comm"));
			comm.push_back(EndPoint(string("us_")+istr,"comm"));
			comm.push_back(EndPoint(string("vs_")+istr,"comm"));
		}

		mpi_management->mpi_connect(comm);

	}

	virtual void process_file ( const char* filename)
	{
		string file(filename);
		std::size_t found=file.find("_",0);
		string width = file.substr(0,found);
		std::size_t found2=file.find("_",found+1);
		string height = file.substr(found+1,found2);
		std::size_t found3=file.find("_",found2+1);
		string it = file.substr(found2+1,found3);
		std::size_t found4=file.find("_",found3+1);
		string proc = file.substr(found3+1,found4);
		create_assembly(width.c_str(),height.c_str(),it.c_str(),proc.c_str());
	}

	virtual void process_data(const char *)
	{

	}
	
};

LCMP(Assembly)
	L_CPP_PROVIDE(XmlParser, parser);
	L_CPP_USE(CppManagement, cpp_management);
	L_CPP_USE(GridInstantiationManagement, grid_instantiation_management);
	L_CPP_USE(MpiManagement, mpi_management);
	L_CPP_USE(TypeSetter, typed_configuration);
LEND

}

}


