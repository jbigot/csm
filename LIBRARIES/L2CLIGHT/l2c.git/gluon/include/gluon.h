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


#ifndef GLUON_GLUON_H
#define GLUON_GLUON_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include <charm++.h>

#include <llcmcpp/component.h>

namespace gluon {

namespace impl {

template < class CProxy_Cmp, class Cmp >
struct Setter
{
	virtual void operator() ( CProxy_Cmp*, Cmp*, void* ) = 0;
	
};

template < class CProxy_Cmp, class Cmp >
struct Getter
{
	virtual void* operator() ( CProxy_Cmp*, Cmp* ) = 0;
	
};

template < class CProxy_Cmp, class Cmp >
class GluonComponent:
		public ::llcmcpp::Component
{
public:
	CProxy_Cmp instance;
	
	virtual Cmp* local_instance () = 0;
	
	const char* name;
	
	GluonComponent ( const char* name ): name(name) {}
	
	typedef std::map< std::string, Setter< CProxy_Cmp, Cmp >* > SetterMap;
	SetterMap input_properties;
	
	typedef std::map< std::string, Getter< CProxy_Cmp, Cmp >* > GetterMap;
	GetterMap output_properties;
	
	void* get(const char* propname)
	{
		typename GetterMap::iterator it = output_properties.find(std::string(propname));
		if ( it == output_properties.end() ) {
			if ( std::string("lifecycle") != propname ) {
				std::cerr << '[' << name << "] *** Error: output property \""<<propname<<"\" does not exist!" << std::endl;
				std::abort();
			}
			return 0;
		}
		return (*(it->second))(&instance, local_instance());
	}
	
	virtual void set(const char* propname, void* value)
	{
		typename SetterMap::iterator it = input_properties.find(std::string(propname));
		if ( it == input_properties.end() ) {
			std::cerr << '[' << name << "] *** Error: input property \""<<propname<<"\" does not exist!" << std::endl;
			std::abort();
		}
		(*(it->second))(&instance, local_instance(), value);
	}
	
};

template < class CProxy_Cmp, class Cmp >
class ChareComponent:
		public GluonComponent< CProxy_Cmp, Cmp >
{
public:
	ChareComponent ( char const * name ):
			GluonComponent< CProxy_Cmp, Cmp >(name)
	{
		this->instance = CProxy_Cmp::ckNew(CkMyPe());
		
	}
	
	virtual Cmp* local_instance ()
	{
		return this->instance.ckLocal();
	}
	
};

template < class CProxy_Cmp, class Cmp >
class ArrayComponent:
		public GluonComponent< CProxy_Cmp, Cmp >
{
public:
	ArrayComponent ( char const * name ):
			GluonComponent< CProxy_Cmp, Cmp >(name)
	{
	}
	
	CkArrayOptions opts;
	
	virtual void* get(const char* propname)
	{
		if ( std::string("__llcmcpp__gluon__arrayid") == propname ) {
			if ( this->instance.ckGetArrayID().isZero() ) {
				this->instance = CProxy_Cmp::ckNew(opts);
			}
			return new CkArrayID(this->instance.ckGetArrayID());
		} else {
			return GluonComponent< CProxy_Cmp, Cmp >::get(propname);
		}
	}
	
	virtual void set(const char* propname, void* value)
	{
		if  ( std::string("__llcmcpp__gluon__arraybind") == propname ) {
			this->opts.bindTo(*static_cast<CkArrayID*>(value));
		} else if  ( std::string("__llcmcpp__gluon__arraysize") == propname ) {
			this->opts.setNumInitial(*static_cast<uint32_t*>(value));
		} else {
			GluonComponent< CProxy_Cmp, Cmp >::set(propname, value);
		}
	}
	
	virtual Cmp* local_instance ()
	{
		return 0;
	}
	
};

template < class CProxy_Cmp, class Cmp >
class GroupComponent:
		public GluonComponent< CProxy_Cmp, Cmp >
{
public:
	GroupComponent ( char const * name ):
			GluonComponent< CProxy_Cmp, Cmp >(name)
	{
	}
	
	virtual void* get(const char* propname)
	{
		if ( std::string("__llcmcpp__gluon__groupid") == propname ) {
			return new CkGroupID(CProxy_Cmp::ckNew());
		}
		return GluonComponent< CProxy_Cmp, Cmp >::get(propname);
	}
	
	virtual void set(const char* propname, void* value)
	{
		if ( std::string("__llcmcpp__gluon__groupid") == propname ) {
			this->instance = *static_cast<CkGroupID*>(value);
		} else {
			GluonComponent< CProxy_Cmp, Cmp >::set(propname, value);
		}
	}
	
	virtual Cmp* local_instance ()
	{
		return this->instance.ckLocalBranch();
	}
	
};



template < class CProxy_Cmp, class Cmp, class T >
struct AttributeProperty:
		Setter< CProxy_Cmp, Cmp >
{
	AttributeProperty ( T Cmp::* dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp* l, void* v )
	{
		l->*m_dat = *(static_cast< T* >(v));
	}
	
	T Cmp::*m_dat;
	
};

template < class CProxy_Cmp, class Cmp, class T >
struct SetterProperty:
		public Setter< CProxy_Cmp, Cmp >
{
	SetterProperty ( void (Cmp::*mth) ( T ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp* l, void* v )
	{
		(l->*m_mth)(*static_cast< T* >(v));
	}
	
	void (Cmp::*m_mth) ( T );
	
};

template < class CProxy_Cmp, class Cmp, class T >
struct CharmSetterProperty:
		public Setter< CProxy_Cmp, Cmp >
{
	CharmSetterProperty ( void (CProxy_Cmp::*mth) ( const T&, const CkEntryOptions * )  ):
			m_mth(mth)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp* l, void* v )
	{
		(c->*m_mth)(*static_cast< T* >(v), 0);
	}
	
	void (CProxy_Cmp::*m_mth) ( const T&, const CkEntryOptions * );
	
};



template < class CProxy_Cmp, class Cmp, class T >
struct InheritanceCppProvide:
		public Getter< CProxy_Cmp, Cmp >
{
	virtual void* operator() ( CProxy_Cmp* c, Cmp* l )
	{
		return static_cast<T*>(l);
	}
	
};

template < class CProxy_Cmp, class Cmp, class T >
struct InheritanceCharmProvide:
		public Getter< CProxy_Cmp, Cmp >
{
	virtual void* operator() ( CProxy_Cmp* c, Cmp* l )
	{
		CkChareID* result = new CkChareID;
		*result = c->ckGetChareID();
		return result;
	}
	
};

template < class CProxy_Cmp, class Cmp, class T >
struct InheritanceCharmAProvide:
		public Getter< CProxy_Cmp, Cmp >
{
	virtual void* operator() ( CProxy_Cmp* c, Cmp* l )
	{
		CkArrayID* result = new CkArrayID;
		*result = c->ckGetArrayID();
		return result;
	}
	
};

template < class CProxy_Cmp, class Cmp, class T >
struct InheritanceCharmGProvide:
		public Getter< CProxy_Cmp, Cmp >
{
	virtual void* operator() ( CProxy_Cmp* c, Cmp* l )
	{
		CkGroupID* result = new CkGroupID;
		*result = static_cast<T*>(c)->ckGetGroupID();
		return result;
	}
	
};



template < class CProxy_Cmp, class Cmp, class T >
struct AttributeCppUse:
		public Setter< CProxy_Cmp, Cmp >
{
	AttributeCppUse ( T* Cmp::*dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp* l, void* v )
	{
		l->*m_dat = static_cast< T* >(v);
	}
	
	T* Cmp::*m_dat;
	
};

template < class CProxy_Cmp, class Cmp, class T >
struct SetterCppUse:
		public Setter< CProxy_Cmp, Cmp >
{
	SetterCppUse ( void (Cmp::*mth) ( T* ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp* l, void* v )
	{
		(l->*m_mth)(static_cast< T* >(v));
	}
	
	void (Cmp::*m_mth) ( T* );
	
};

template < class CProxy_Cmp, class Cmp, class T, class CompoID >
struct AttributeCharmUse:
		public Setter< CProxy_Cmp, Cmp >
{
	AttributeCharmUse ( T Cmp::*dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp* l, void* v )
	{
		(l->*m_dat)= *static_cast< CompoID* >(v);
	}
	
	T Cmp::*m_dat;
	
};

template < class CProxy_Cmp, class Cmp, class T, class CompoID >
struct SetterCharmUse:
		public Setter< CProxy_Cmp, Cmp >
{
	SetterCharmUse ( void (CProxy_Cmp::*mth) ( const T&, const CkEntryOptions * ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( CProxy_Cmp* c, Cmp*, void* v )
	{
		(c->*m_mth)(T(*static_cast< CompoID* >(v)), 0);
	}
	
	void (CProxy_Cmp::*m_mth) ( const T&, const CkEntryOptions * );
	
};



template < class CProxy_Cmp, class Cmp, class T >
void property ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, void (Cmp::*mth) ( T ) )
{
	cmp->input_properties[name] = new SetterProperty< CProxy_Cmp, Cmp, T >(mth);
}

template < class CProxy_Cmp, class Cmp, class T >
void property ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, void (CProxy_Cmp::*mth) ( const T&, const CkEntryOptions * ) )
{
	cmp->input_properties[name] = new CharmSetterProperty< CProxy_Cmp, Cmp, T >(mth);
}

template < class CProxy_Cmp, class Cmp, class T >
void property ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, T Cmp::* dat )
{
	cmp->input_properties[name] = new AttributeProperty< CProxy_Cmp, Cmp, T >(dat);
}



template < class CProxy_Cmp, class Cmp, class T >
void cpp_provide ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name )
{
	cmp->output_properties[name] = new InheritanceCppProvide< CProxy_Cmp, Cmp, T >;
}

template < class CProxy_Cmp, class Cmp, class T >
void charm_provide ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name )
{
	cmp->output_properties[name] = new InheritanceCharmProvide< CProxy_Cmp, Cmp, T >;
}

template < class CProxy_Cmp, class Cmp, class T >
void charm_aprovide ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name )
{
	cmp->output_properties[name] = new InheritanceCharmAProvide< CProxy_Cmp, Cmp, T >;
}

template < class CProxy_Cmp, class Cmp, class T >
void charm_gprovide ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name )
{
	cmp->output_properties[name] = new InheritanceCharmGProvide< CProxy_Cmp, Cmp, T >;
}



template < class CProxy_Cmp, class Cmp, class T >
void cpp_use ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, void (Cmp::*mth) ( T* ) )
{
	cmp->input_properties[name] = new SetterCppUse< CProxy_Cmp, Cmp, T >(mth);
}

template < class CProxy_Cmp, class Cmp, class T >
void cpp_use ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, T* Cmp::* attr )
{
	cmp->input_properties[name] = new AttributeCppUse< CProxy_Cmp, Cmp, T >(attr);
}

template < class CProxy_Cmp, class Cmp, class T, class CompoID >
void charm_use ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, void (CProxy_Cmp::*mth) ( const T&, const CkEntryOptions * ) )
{
	cmp->input_properties[name] = new SetterCharmUse< CProxy_Cmp, Cmp, T, CompoID >(mth);
}

template < class CProxy_Cmp, class Cmp, class T, class CompoID >
void charm_use ( GluonComponent< CProxy_Cmp, Cmp >* cmp, std::string name, T Cmp::*attr )
{
	cmp->input_properties[name] = new AttributeCharmUse< CProxy_Cmp, Cmp, T, CompoID >(attr);
}



}

}

#define GCMP(type) \
extern "C" void _llcmcpp_register_##type () { \
	_register##type ();\
}\
extern "C" ::llcmcpp::Component* _llcmcpp_create_##type () { \
	typedef CProxy_##type CProxy_Cmp;\
	typedef type Cmp;\
	typedef CkIndex_##type CkIndex_Cmp;\
	typedef CkChareID CompoID;\
	::gluon::impl::GluonComponent< CProxy_Cmp, Cmp >* cmp = new ::gluon::impl::ChareComponent< CProxy_Cmp, Cmp >( #type );



#define GCMP_A(type) \
extern "C" void _llcmcpp_register_##type () { \
	_register##type ();\
}\
extern "C" ::llcmcpp::Component* _llcmcpp_create_##type () { \
	typedef CProxy_##type CProxy_Cmp;\
	typedef type Cmp;\
	typedef CkIndex_##type CkIndex_Cmp;\
	typedef CkGroupID CompoID;\
	::gluon::impl::GluonComponent< CProxy_Cmp, Cmp >* cmp = new ::gluon::impl::ArrayComponent< CProxy_Cmp, Cmp >( #type );



#define GCMP_G(type) \
extern "C" void _llcmcpp_register_##type () { \
	_register##type ();\
}\
extern "C" ::llcmcpp::Component* _llcmcpp_create_##type () { \
	typedef CProxy_##type CProxy_Cmp;\
	typedef type Cmp;\
	typedef CkIndex_##type CkIndex_Cmp;\
	typedef CkGroupID CompoID;\
	::gluon::impl::GluonComponent< CProxy_Cmp, Cmp >* cmp = new ::gluon::impl::GroupComponent< CProxy_Cmp, Cmp >( #type );



#define G_PROPERTY(type, name) \
	::gluon::impl::property< CProxy_Cmp, Cmp, type >(cmp, #name, &Cmp::name);

#define G_PROPERTY2(type, name) \
	::gluon::impl::property< CProxy_Cmp, Cmp, type >(cmp, #name, &CProxy_Cmp::name);



#define G_CPP_PROVIDE(type, name) \
	::gluon::impl::cpp_provide< CProxy_Cmp, Cmp, type >(cmp, #name);

#define G_CHARM_PROVIDE(type, name) \
	::gluon::impl::charm_provide< CProxy_Cmp, Cmp, CProxy_##type >(cmp, #name);

#define G_CHARM_APROVIDE(type, name) \
	::gluon::impl::charm_aprovide< CProxy_Cmp, Cmp, CProxy_##type >(cmp, #name);

#define G_CHARM_GPROVIDE(type, name) \
	::gluon::impl::charm_gprovide< CProxy_Cmp, Cmp, CProxy_##type >(cmp, #name);



#define G_CPP_USE(type, name) \
	::gluon::impl::cpp_use< CProxy_Cmp, Cmp, type >(cmp, #name, &Cmp::name);

#define G_CHARM_USE(type, name) \
	::gluon::impl::charm_use< CProxy_Cmp, Cmp, CProxy_##type, CkChareID >(cmp, #name, &Cmp::name);

#define G_CHARM_USE2(type, name) \
	::gluon::impl::charm_use< CProxy_Cmp, Cmp, CProxy_##type, CkChareID >(cmp, #name, &CProxy_Cmp::name);

#define G_CHARM_AUSE(type, name) \
	::gluon::impl::charm_use< CProxy_Cmp, Cmp, CProxy_##type, CkArrayID >(cmp, #name, &Cmp::name);

#define G_CHARM_AUSE2(type, name) \
	::gluon::impl::charm_use< CProxy_Cmp, Cmp, CProxy_##type, CkArrayID >(cmp, #name, &CProxy_Cmp::name);

#define G_CHARM_GUSE(type, name) \
	::gluon::impl::charm_use< CProxy_Cmp, Cmp, CProxy_##type, CkGroupID >(cmp, #name, &Cmp::name);

#define G_CHARM_GUSE2(type, name) \
	::gluon::impl::charm_use< CProxy_Cmp, Cmp, CProxy_##type, CkGroupID >(cmp, #name, &CProxy_Cmp::name);



#define GEND return cmp; }



#endif // GLUON_GLUON_H
