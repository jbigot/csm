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
#include <string>

#include <go.h>
#include <llcmcpp3.h>
#include <rmimanagement.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class RmiManagementBuffer:
		virtual public Go,
		virtual public RmiManagement
{
public:
	RmiManagement* rmi_management;
	
	RmiManagementBuffer ():
			rmi_management(0)
	{}
	
    virtual void connect ( EndPoint user, EndPoint provider )
    {
		m_buffer.push_back(RmiConnection(user, provider));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[RmiManagementBuffer] connecting!" << endl;
#endif
		for ( list< RmiConnection >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			rmi_management->connect(it->user, it->provider);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[RmiManagementBuffer] connections done!" << endl;
#endif
	}
	
private:
	struct RmiConnection {
		EndPoint user; EndPoint provider;
		RmiConnection ( EndPoint user, EndPoint provider ): user(user), provider(provider) {}
	};
	
	list< RmiConnection > m_buffer;
	
};

LCMP(RmiManagementBuffer)
	L_CPP_PROVIDE(RmiManagement, rmi_manager);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(RmiManagement, rmi_management);
LEND

}

}

