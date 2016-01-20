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

#ifndef LLCMCPP_LLCMCPP3_H
#define LLCMCPP_LLCMCPP3_H

#include <string>

#include "llcmcommon.h"

namespace llcmcpp {

namespace impl {

template < class Cmp, class C >
struct SetterCppUse:
		public Setter< Cmp >
{
	SetterCppUse ( void (Cmp::*mth) ( C* ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		(c->*m_mth)(static_cast< C* >(v));
	}
	
	void (Cmp::*m_mth) ( C* );
	
};

template < class Cmp, class C >
struct AttributeCppUse:
		public Setter< Cmp >
{
	AttributeCppUse ( C* Cmp::*dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		c->*m_dat = static_cast< C* >(v);
	}
	
	C* Cmp::*m_dat;
	
};

template < class Cmp, class C >
struct SetterProperty:
		public Setter< Cmp >
{
	SetterProperty ( void (Cmp::*mth) ( C ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		(c->*m_mth)(*static_cast< C* >(v));
	}
	
	void (Cmp::*m_mth) ( C );
	
};

template < class Cmp, class C >
struct AttributeProperty:
		Setter< Cmp >
{
	AttributeProperty ( C Cmp::* dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		c->*m_dat = *(static_cast< C* >(v));
	}
	
	C Cmp::*m_dat;
	
};

template < class Cmp, class C >
struct GetterCppProvide:
		Getter< Cmp >
{
	GetterCppProvide ( C* (Cmp::*mth) () ):
			m_mth(mth)
	{}
	
	virtual void* operator() ( Cmp* c )
	{
		return (c->*m_mth)();
	}
	
	C* (Cmp::*m_mth) ();
	
};

template < class Cmp, class C >
struct InheritanceCppProvide:
		public Getter< Cmp >
{
	virtual void* operator() ( Cmp* c )
	{
		return static_cast<C*>(c);
	}
	
};

template < class Cmp, class C, class SubC >
struct AttributeCppProvide:
		public Getter< Cmp >
{
	AttributeCppProvide ( SubC Cmp::* dat ):
			m_dat(dat)
	{}
	
	virtual void* operator() ( Cmp* c )
	{
		return static_cast<C*>(&(c->*m_dat));
	}
	
	SubC Cmp::*m_dat;
	
};

template < class Cmp, class C >
void property ( BasicComponent< Cmp >* cmp, std::string name, void (Cmp::*mth) ( C ) )
{
	cmp->input_properties[name] = new SetterProperty< Cmp, C >(mth);
}

template < class Cmp, class C >
void property ( BasicComponent< Cmp >* cmp, std::string name, C Cmp::* dat )
{
	cmp->input_properties[name] = new AttributeProperty< Cmp, C >(dat);
}

template < class Cmp, class C >
void cpp_use ( BasicComponent< Cmp >* cmp, std::string name, void (Cmp::*mth) ( C* ) )
{
	cmp->input_properties[name] = new SetterCppUse< Cmp, C >(mth);
}

template < class Cmp, class C >
void cpp_use ( BasicComponent< Cmp >* cmp, std::string name, C* Cmp::* dat )
{
	cmp->input_properties[name] = new AttributeCppUse< Cmp, C >(dat);
}

template < class Cmp, class C >
void cpp_provide ( BasicComponent< Cmp >* cmp, std::string name,  C* (Cmp::*mth) () )
{
	cmp->output_properties[name] = new GetterCppProvide< Cmp, C >(mth);
}

template < class Cmp, class C, class SubC >
void cpp_provide ( BasicComponent< Cmp >* cmp, std::string name, SubC Cmp::* dat )
{
	cmp->output_properties[name] = new AttributeCppProvide< Cmp, C, SubC >(dat);
}

template < class Cmp, class C >
void cpp_provide ( BasicComponent< Cmp >* cmp, std::string name )
{
	cmp->output_properties[name] = new InheritanceCppProvide< Cmp, C >;
}
  
}

}

#define L_PROPERTY(type, name) \
	::llcmcpp::impl::property< Cmp, type >(cmp, #name, &Cmp::name);

#define L_CPP_USE(type, name) \
	::llcmcpp::impl::cpp_use< Cmp, type >(cmp, #name, &Cmp::name);

#define L_CPP_PROVIDE(type, name) \
	::llcmcpp::impl::cpp_provide< Cmp, type >(cmp, #name);

#define L_CPP_PROVIDE2(type, name) \
	::llcmcpp::impl::cpp_provide< Cmp, type >(cmp, #name, &Cmp::name);

#endif // LLCMCPP_LLCMCPP3_H
