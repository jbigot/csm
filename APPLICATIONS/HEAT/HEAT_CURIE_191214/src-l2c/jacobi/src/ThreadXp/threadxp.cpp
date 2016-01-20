#include <iostream>
#include <list>
#include <numeric>
#include <string>
#include <vector>

#include <boost/thread/barrier.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <checkdata.h>
#include <computerniter.h>
#include <memory.h>
#include <XPcomputer.h>

namespace jacobi {

namespace impl {

using ::boost::condition_variable;
using ::boost::lock_guard;
using ::boost::mutex;
using ::boost::thread_group;
using ::boost::unique_lock;
using ::std::accumulate;
using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::max;
using ::std::string;
using ::std::swap;
using ::std::vector;

struct MemAlloc { double* from; double* to; MemAlloc(double* from, double* to): from(from), to(to) {} };

class ThreadXp:
	virtual public XPComputer
{
public:
	uint16_t global_nbtiles_x; ///< nb of memory tiles in the global space
	uint16_t global_nbtiles_y; ///< nb of memory tiles in the global space
	uint16_t global_tile_x; ///< rank of this memory tile in the global space
	uint16_t global_tile_y; ///< rank of this memory tile in the global space
	
	uint16_t memory_nbtiles_x; ///< nb of thread tiles in the memory space
	uint16_t memory_nbtiles_y; ///< nb of thread tiles in the memory space
	
	string name;
	
	CheckData* checkdata;
	
	void computers ( ComputerNiter* computer )
	{
		m_computers.push_back(computer);
	}
	vector< ComputerNiter* > m_computers;
	
	void memory_allocators ( Memory* memory_allocator )
	{
		m_memory_allocators.push_back(memory_allocator);
	}
	vector< Memory* > m_memory_allocators;
	
	ThreadXp ():
			checkdata(0),
			global_nbtiles_x(0),
			global_nbtiles_y(0),
			global_tile_x(0),
			global_tile_y(0),
			memory_nbtiles_x(0),
			memory_nbtiles_y(0),
			name("ThreadXp")
	{
	}
	
	class Worker
	{
	public:
		uint64_t duration;
		
		Worker ( ComputerNiter* computer, double* from, double* to, size_t memory_rwidth, size_t memory_rheight, size_t tile_rx, size_t tile_ry, size_t tile_rwidth, size_t tile_rheight, long int niter, uint64_t& result):
				m_computer(computer),
				m_from(from),
				m_to(to),
				m_memory_rwidth(memory_rwidth),
				m_memory_rheight(memory_rheight),
				m_tile_rx(tile_rx),
				m_tile_ry(tile_ry),
				m_tile_rwidth(tile_rwidth),
				m_tile_rheight(tile_rheight),
				m_niter(niter),
				m_result(result)
		{
		}
		
		void operator() ()
		{
#ifndef NDEBUG
			cerr << "[ThreadXp] n_iter("<<m_from<<", "<<m_to<<", "<<m_memory_rwidth<<", "<<m_memory_rheight<<", "<<m_tile_rx<<", "<<m_tile_ry<<", "<<m_tile_rwidth<<", "<<m_tile_rheight<<", "<<m_niter<<')' << endl;
#endif //NDEBUG
			m_result = m_computer->n_iter(m_from, m_to, m_memory_rwidth, m_memory_rheight, m_tile_rx, m_tile_ry, m_tile_rwidth, m_tile_rheight, m_niter);
		}
		
	private:
		ComputerNiter* m_computer;

		double* m_from;
		double* m_to;
		size_t m_memory_rwidth;
		size_t m_memory_rheight;
		size_t m_tile_rx;
		size_t m_tile_ry;
		size_t m_tile_rwidth;
		size_t m_tile_rheight;
		long m_niter;
		uint64_t& m_result;
		
	};

	
	uint64_t compute( size_t global_rwidth, size_t global_rheight, unsigned long niter )
	{
#ifndef NDEBUG
		cerr << "[ThreadXp] compute(" << global_rwidth << ", " << global_rheight << ", " << niter << ')' << endl;
#endif //NDEBUG
		
		if ( m_memory_allocators.size() != 1 && m_memory_allocators.size() != m_computers.size() ) {
			cerr << "[ThreadXp] *** error: wrong number of memory allocators! ("<<m_memory_allocators.size()<<")" << endl;
			abort();
		}
		
		if ( m_computers.size() != memory_nbtiles_x*memory_nbtiles_y ) {
			cerr << "[ThreadXp] *** error: not enough computer wrt. nbtiles!" << endl;
			cerr << "[ThreadXp] *** "<< m_computers.size()<<" <> "<<memory_nbtiles_x<<"x"<<memory_nbtiles_y<<endl;
			abort();
		}
		
		// size of this process_tile
		size_t memory_rwidth = ((global_rwidth-2)/global_nbtiles_x)+2;
		size_t memory_rheight = ((global_rheight-2)/global_nbtiles_y)+2;
		if ( global_tile_x == global_nbtiles_x-1 ) memory_rwidth = global_rwidth - global_tile_x*(memory_rwidth-2);
		if ( global_tile_y == global_nbtiles_y-1 ) memory_rheight = global_rheight - global_tile_y*(memory_rheight-2);
		
		double* from;
		double* to;
		
		list< MemAlloc > cleanup;
		vector< Memory* >::const_iterator alloc_it = m_memory_allocators.begin();
		
		if ( m_memory_allocators.size() == 1 ) {
			(*alloc_it)->alloc(from, to, memory_rwidth, memory_rheight);
			cleanup.push_back(MemAlloc(from, to));
		}
		
		thread_group threads;
		uint64_t* durations = new uint64_t[memory_nbtiles_x*memory_nbtiles_y];
		
		uint64_t* this_duration = durations;
		vector< ComputerNiter* >::iterator comp_it = m_computers.begin();
		size_t tile_basic_rwidth = ((memory_rwidth-2)/memory_nbtiles_x)+2;
		size_t tile_basic_rheight = ((memory_rheight-2)/memory_nbtiles_y)+2;
		for ( unsigned memory_tile_x=0; memory_tile_x<memory_nbtiles_x; ++memory_tile_x) {
			for ( unsigned memory_tile_y=0; memory_tile_y<memory_nbtiles_y; ++memory_tile_y) {
				// size of this thread tile
				size_t tile_rwidth = tile_basic_rwidth;
				size_t tile_rheight = tile_basic_rheight;
				if ( memory_tile_x == memory_nbtiles_x-1 ) tile_rwidth = memory_rwidth - memory_tile_x*(tile_basic_rwidth-2);
				if ( memory_tile_y == memory_nbtiles_y-1 ) tile_rheight = memory_rheight - memory_tile_y*(tile_basic_rheight-2);
				
				if (( m_memory_allocators.size() == m_computers.size() ) && (m_memory_allocators.size() >1)) {
					(*alloc_it)->alloc(from, to, tile_rwidth, tile_rheight);
					cleanup.push_back(MemAlloc(from, to));
					++alloc_it;
				}
				
				if ( m_memory_allocators.size() == 1 ) threads.create_thread(Worker(*comp_it, from, to, memory_rwidth, memory_rheight, memory_tile_x*(tile_basic_rwidth-2), memory_tile_y*(tile_basic_rheight-2), tile_rwidth, tile_rheight, niter, *this_duration));
				
				if ( m_memory_allocators.size() == m_computers.size() ) threads.create_thread(Worker(*comp_it, from, to, tile_rwidth, tile_rheight, 0, 0, tile_rwidth, tile_rheight, niter, *this_duration));
				
				++this_duration;
				++comp_it;
			}
		}
		
		threads.join_all();
		
		if ( m_memory_allocators.size() == 1 ) {
			if (checkdata) checkdata->compare(ArrayPart<double>(from, memory_rwidth, memory_rheight, 1, 1, memory_rwidth-2, memory_rheight-2), name);
		}
		
		alloc_it = m_memory_allocators.begin();
		for ( list< MemAlloc >::iterator cl_it = cleanup.begin(); cl_it != cleanup.end(); ++cl_it ) {
			(*alloc_it)->dealloc(cl_it->from, cl_it->to);
			++alloc_it;
		}
		
		uint64_t result = 0; 
		for ( uint64_t* oneduration = durations ;  oneduration < durations+memory_nbtiles_x*memory_nbtiles_y ; ++oneduration ) {
			result = max( *oneduration, result);	
		}
		delete[] durations;
		return result;
	}
	
};

LCMP(ThreadXp)
	L_PROPERTY(uint16_t, global_nbtiles_x); ///< nb of memory tiles in the global space
	L_PROPERTY(uint16_t, global_nbtiles_y); ///< nb of memory tiles in the global space
	L_PROPERTY(uint16_t, global_tile_x); ///< rank of this memory tile in the global space
	L_PROPERTY(uint16_t, global_tile_y); ///< rank of this memory tile in the global space
	L_PROPERTY(uint16_t, memory_nbtiles_x); ///< nb of thread tiles in the memory space
	L_PROPERTY(uint16_t, memory_nbtiles_y); ///< nb of thread tiles in the memory space
	L_PROPERTY(string, name);
	L_CPP_PROVIDE(XPComputer, xpcomputer);
	L_CPP_USE(CheckData, checkdata);
	L_CPP_USE(ComputerNiter, computers);
	L_CPP_USE(Memory, memory_allocators);
LEND

}

}
