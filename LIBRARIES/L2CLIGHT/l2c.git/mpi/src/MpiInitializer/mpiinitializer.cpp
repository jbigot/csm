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

#include <mpi.h>

#include <iostream>
#include <string>


#include <stdint.h>

#include <initializer.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::string;

class MpiInitializer:
		virtual public Initializer
{
public:
	string thread_level;
	
	MPI_Comm world;
	
	MpiInitializer ():
		thread_level("SINGLE"),
		world(MPI_COMM_WORLD)
	{}
	
	~MpiInitializer ()
	{
		MPI_Finalize();
	}
	
	static int mpi_level( string level )
	{
		if ( level == "SINGLE" ) return MPI_THREAD_SINGLE;
		if ( level == "SERIALIZED" ) return MPI_THREAD_SERIALIZED;
		if ( level == "FUNNELED" ) return MPI_THREAD_FUNNELED;
		if ( level == "MULTIPLE" ) return MPI_THREAD_MULTIPLE;
		cerr << "[MpiInitializer] *** Error: unknown thread level: "<<level<<endl;
		cerr << "                 valid levels are SINGLE, SERIALIZED, FUNNELED, MULTIPLE"<<endl;
		abort();
	}
	
	static string mpi_level( int level )
	{
		switch ( level ) {
			case MPI_THREAD_SINGLE: return "SINGLE";
			case MPI_THREAD_SERIALIZED: return "SERIALIZED";
			case MPI_THREAD_FUNNELED: return "FUNNELED";
			case MPI_THREAD_MULTIPLE: return "MULTIPLE";
			default: {
				cerr << "[MpiInitializer] *** Error: unknown thread level: "<<level<<endl;
				abort();
			} break;
		}
	}
	
	virtual void initialize ( int* argc, char*** argv )
	{
#ifndef NDEBUG
		cerr << "[MpiInitializer] initialize" << endl;
		cerr << "[MpiInitializer] before MPI, argc=" << *argc << endl;
		cerr << "[MpiInitializer] before MPI, argv=| ";
		for ( int ii=0; ii<*argc; ++ii ) {
			cerr << (*argv)[ii] << " | ";
		}
		cerr << endl;
#endif // NDEBUG
		int required = mpi_level(thread_level);
		int provided;
		MPI_Init_thread(argc, argv, required, &provided);
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if ( rank == 0 && provided < required ) {
			cerr << "MPI thread support level "<<thread_level<<" required but "<<mpi_level(provided)<<" found, aborting"<<endl;
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}
};

LCMP(MpiInitializer)
	L_PROPERTY(string, thread_level);
	L_CPP_PROVIDE(Initializer, initializer);
	L_CPP_PROVIDE2(MPI_Comm, world);
LEND

}

}
