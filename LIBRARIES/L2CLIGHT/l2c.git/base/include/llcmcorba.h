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

#ifndef LLCMCPP_LLCMCORBA_H
#define LLCMCPP_LLCMCORBA_H

#include <string>

#include <omniORB4/CORBA.h>

#include "llcmcommon.h"

namespace llcmcpp {

namespace impl {

template < class Cmp, class C >
struct SetterCorbaUse:
		public Setter< Cmp >
{
	SetterCorbaUse ( void (Cmp::*mth) ( typename C::_ptr_type ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		(c->*m_mth)(C::_narrow(static_cast< ::CORBA::Object_ptr >(v)));
	}
	
	void (Cmp::*m_mth) ( typename C::_ptr_type );
	
};

template < class Cmp, class C >
struct AttributeCorbaUse:
		public Setter< Cmp >
{
	AttributeCorbaUse ( typename C::_ptr_type Cmp::*dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		c->*m_dat = (C::_narrow(static_cast< ::CORBA::Object_ptr >(v)));
	}
	
	typename C::_ptr_type Cmp::*m_dat;
	
};

template < class Cmp, class C >
struct GetterCorbaProvide:
		Getter< Cmp >
{
	GetterCorbaProvide ( typename C::_ptr_type (Cmp::*mth) () ):
			m_mth(mth)
	{}
	
	virtual void* operator() ( Cmp* c )
	{
		return ::CORBA::Object::_narrow((c->*m_mth)());
	}
	
	typename C::_ptr_type (Cmp::*m_mth) ();
	
};

template < class Cmp, class C >
struct AttributeCorbaProvide:
		public Getter< Cmp >
{
	AttributeCorbaProvide ( C Cmp::* dat ):
			m_dat(dat)
	{}
	
	virtual void* operator() ( Cmp* c )
	{
		return ::CORBA::Object::_narrow(&(c->*m_dat));
	}
	
	C Cmp::*m_dat;
	
};

template < class Cmp, class C >
struct InheritanceCorbaProvide:
		public Getter< Cmp >
{
	virtual void* operator() ( Cmp* c )
	{
		return ::CORBA::Object::_narrow(c->_this());
	}
	
};

template < class Cmp, class C >
void corba_use ( BasicComponent< Cmp >* cmp, std::string name, void (Cmp::*mth) ( typename C::_ptr_type ) )
{
	cmp->input_properties[name] = new SetterCorbaUse< Cmp, C >(mth);
}

template < class Cmp, class C >
void corba_use ( BasicComponent< Cmp >* cmp, std::string name, typename C::_ptr_type Cmp::* dat )
{
	cmp->input_properties[name] = new AttributeCorbaUse< Cmp, C >(dat);
}

template < class Cmp, class C >
void corba_provide ( BasicComponent< Cmp >* cmp, std::string name, typename C::_ptr_type (Cmp::*mth) () )
{
	cmp->output_properties[name] = new GetterCorbaProvide< Cmp, C >(mth);
}

template < class Cmp, class C >
void corba_provide ( BasicComponent< Cmp >* cmp, std::string name, C Cmp::* dat )
{
	cmp->output_properties[name] = new AttributeCorbaProvide< Cmp, C >(dat);
}

template < class Cmp, class C >
void corba_provide ( BasicComponent< Cmp >* cmp, std::string name )
{
	cmp->output_properties[name] = new InheritanceCorbaProvide< Cmp, C >;
}

}

}

#define L_CORBA_USE(type, name) \
	::llcmcpp::impl::corba_use< Cmp, type >(cmp, #name, &Cmp::name);

#define L_CORBA_PROVIDE(type, name) \
	::llcmcpp::impl::corba_provide< Cmp, type >(cmp, #name);

#define L_CORBA_PROVIDE2(type, name) \
	::llcmcpp::impl::corba_provide< Cmp, type >(cmp, #name, &Cmp::name);

#endif // LLCMCPP_LLCMCORBA_H
