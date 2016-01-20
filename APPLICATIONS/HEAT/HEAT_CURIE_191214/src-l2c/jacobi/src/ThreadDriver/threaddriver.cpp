#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/thread/barrier.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <llcmcpp/lifecycle.h>
#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <checkdata.h>
#include <computer.h>
#include <initializer.h>
#include <XPcomputer.h>


typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))
#define timing_usec(a) (((long long)(a).tv_sec)*1000000+((long long)(a).tv_usec))


#define FROM(y,x) A_from[ (x) + w * (y) ]
#define RESULT(y,x) A_result[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]


using ::boost::barrier;
using ::boost::condition_variable;
using ::boost::lock_guard;
using ::boost::mutex;
using ::boost::thread_group;
using ::boost::unique_lock;
using ::llcmcpp::LifeCycle;
using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::swap;
using ::std::vector;

namespace jacobi {

namespace impl {

class ThreadDriver :
		virtual public XPComputer,
		virtual public LifeCycle
{
public:
	uint8_t debug;
	
	uint16_t nb_tiles_x, nb_tiles_y;
	
	CheckData* checkdata;
	
	void computers ( Computer* computer )
	{
		m_computers.push_back(computer);
	}
	vector< Computer* > m_computers;
	
	void initializations ( Initializer* initialization )
	{
		m_initializations.push_back(initialization);
	}
	vector< Initializer* > m_initializations;
	
	ThreadDriver ():
			debug(0xFF),
			nb_tiles_x(1),
			nb_tiles_y(1),
			checkdata(0)
	{
	}
	
private:
	class Worker
	{
	public:
		Computer* computer;
		Initializer* initialization;
		barrier& iter_barrier;
		int id;
		
		mutex m_mutex;
		condition_variable m_cond;
		enum { STOPPED, INIT, INITIALIZED, RUN } m_status;
		
		double* m_A_from;
		double* m_A_to;
		size_t m_width;
		size_t m_height;
		size_t m_tile_x;
		size_t m_tile_y;
		size_t m_tile_width;
		size_t m_tile_height;
		long m_niter;

		//my_time_t tick_start, tick_end;

		Worker ( Computer* computer, Initializer* initialization, barrier& iter_barrier, int id ):
				computer(computer),
				initialization(initialization),
				iter_barrier(iter_barrier),
				m_status(STOPPED),
				id(id)
		{}
		
		void init ( double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_x, const size_t tile_y, const size_t tile_width, const size_t tile_height, long niter )
		{
			unique_lock<mutex> l(m_mutex);
			while ( m_status != STOPPED ) m_cond.wait(l);
			m_A_from = A_from;
			m_A_to = A_to;
			m_width = width;
			m_height = height;
			m_tile_x = tile_x;
			m_tile_y = tile_y;
			m_tile_width = tile_width;
			m_tile_height = tile_height;
			m_niter = niter;
			m_status = INIT;
			m_cond.notify_all();
		}
		
		void prepare_to_run() {
			unique_lock<mutex> l(m_mutex);
#ifndef NDEBUG
			//cerr << "Thread "<<this<<" ts="<<timing_usec(tick_start)<<" te="<<timing_usec(tick_end)<<" tt="<<timing_diff_usec(tick_start, tick_end)<<endl;
#endif // NDEBUG
			while ( m_status != INITIALIZED ) m_cond.wait(l);
		}
		
		void run ()
		{
			unique_lock<mutex> l(m_mutex);
			m_status = RUN;
			m_cond.notify_all();
		}
		
		void operator() ()
		{
			unique_lock<mutex> l(m_mutex);
			for (;;) {
				while ( m_status != INIT ) m_cond.wait(l);
#ifndef NDEBUG
			cerr << "Thread["<<m_tile_x<<", "<<m_tile_y<<"] init("<<m_A_from<<", "<<m_width<<", "<<m_height<<", "<<m_tile_x<<", "<<m_tile_y<<", "<<m_tile_width<<", "<<m_tile_height<<")"<<endl;
#endif // NDEBUG
				
				initialization->init(
						ArrayPart< double >(m_A_from, m_width, m_height, m_tile_x, m_tile_y, m_tile_width, m_tile_height),
						m_tile_x==0,
						m_tile_x+m_tile_width==m_width,
						m_tile_y==0,
						m_tile_y+m_tile_height==m_height
				);
				
				initialization->init(
						ArrayPart< double >(m_A_to, m_width, m_height, m_tile_x, m_tile_y, m_tile_width, m_tile_height),
						m_tile_x==0,
						m_tile_x+m_tile_width==m_width,
						m_tile_y==0,
						m_tile_y+m_tile_height==m_height
				);
				
				// Heating caches !
				iter_barrier.wait();
				for ( int iter=0; iter<std::min(10L, m_niter); ++iter ) {
					computer->one_iter(m_A_from, m_A_to, m_width, m_height, m_tile_x+1, m_tile_y+1, m_tile_width-2, m_tile_height-2 );
					swap(m_A_from, m_A_to);
					iter_barrier.wait();
				}
				
//				cerr << "[Thread] iter="<<m_niter<<". m_width="<<m_width<<", m_height="<<m_height<<", m_tile_x="<<m_tile_x<<", m_tile_y="<<m_tile_y<<", m_tile_width="<<m_tile_width<<", m_tile_height="<<m_tile_height<<"\n";
				
				m_status = INITIALIZED;
				m_cond.notify_all();
				
				while ( m_status != RUN ) m_cond.wait(l);
				
				//get_time ( tick_start );
				
				for ( long iter=0; iter<m_niter; ++iter ) {
					computer->one_iter(m_A_from, m_A_to, m_width, m_height, m_tile_x+1, m_tile_y+1, m_tile_width-2, m_tile_height-2 );
					swap(m_A_from, m_A_to);
					iter_barrier.wait();
				}
				//get_time ( tick_end );
				
				m_status = STOPPED;
				m_cond.notify_all();
				
			}
		}
		
		void join ()
		{
			unique_lock<mutex> l(m_mutex);
			while ( m_status != STOPPED ) m_cond.wait(l);
		}
		
		double* result ()
		{
			unique_lock<mutex> l(m_mutex);
			while ( m_status != STOPPED ) m_cond.wait(l);
			return m_A_from;
		}
	};
	
	class Workload
	{
	public:
		Worker* w;
		Workload(Worker* w): w(w) {}
		Workload(Computer* computer, Initializer* initialization, barrier& iter_barrier, int id): w(new Worker(computer, initialization, iter_barrier, id)) {}
		void operator() () { (*w)(); }
	};
	
public:
	vector< Worker* > m_workers;
	barrier* iter_barrier;
	thread_group threads;
	
	virtual void configure_complete()
	{
		if ( nb_tiles_x*nb_tiles_y != m_computers.size() ) {
			cerr << "[ThreadDriver] *** Error: bad tile numbers: " << nb_tiles_x << "X" << nb_tiles_y << " vs. " << m_computers.size() << endl;
			abort();
		}
		if ( m_initializations.size() != m_computers.size() ) {
			cerr << "[ThreadDriver] *** Error: bad number of initializers: " << m_initializations.size() << " vs. " << m_computers.size() << endl;
			abort();
		}
		iter_barrier = new barrier(m_computers.size());
		for ( int ii=0; ii<m_computers.size(); ++ii) {
			Worker* w = new Worker(m_computers[ii], m_initializations[ii], *iter_barrier, ii);
			threads.create_thread< Workload >(Workload(w));
			m_workers.push_back(w);
		}
		m_computers.clear();
	}
	
	uint64_t compute(size_t w, size_t h, unsigned long niter)
	{
#ifndef NDEBUG
		cerr << "[ThreadDriver] Compute("<<w<<", "<<h<<", "<<niter<<")"<< endl;
#endif // NDEBUG
		
		double* A_from = new double[h*w];
#ifndef NDEBUG
		cerr << "[ThreadDriver] Compute from "<<A_from<<endl;
#endif // NDEBUG
		double* A_to = new double[h*w];
#ifndef NDEBUG
		cerr << "[ThreadDriver] Compute to"<<A_to<<endl;
#endif // NDEBUG
		
		size_t tile_height = (h-2)/nb_tiles_y;
		size_t tile_width = (w-2)/nb_tiles_x;
		
		vector< Worker* >::iterator it = m_workers.begin();
		for ( size_t tile_y = 0; tile_y < nb_tiles_y; ++ tile_y ) {
			for ( size_t tile_x = 0; tile_x < nb_tiles_x; ++ tile_x ) {
				size_t local_width = tile_width;
				if ( tile_x == nb_tiles_x-1 ) local_width = w-2 - tile_x*tile_width;
				size_t local_height = tile_height;
				if ( tile_y == nb_tiles_y-1 ) local_height = h-2 - tile_y*tile_height;
				(*it)->init(A_from, A_to, w, h, tile_x*tile_width, tile_y*tile_height, local_width+2, local_height+2, niter);
				++it;
			}
		}
		
		if ( debug & 1 ) {
			for ( size_t yy=0; yy<h; ++yy) {
				cerr << '[' << yy << "@START]\t";
				for ( size_t xx = 0; xx<w; ++xx ) {
					cerr << FROM(yy, xx) << '\t';
				}
				cerr << endl;
			}
			cerr << " ================================ " << endl;
		}
		
		for ( vector< Worker* >::iterator it = m_workers.begin(); it != m_workers.end(); ++it ) (*it)->prepare_to_run();

		my_time_t tick_start;
		get_time ( tick_start );
		
		for ( vector< Worker* >::iterator it = m_workers.begin(); it != m_workers.end(); ++it ) (*it)->run();
		for ( vector< Worker* >::iterator it = m_workers.begin(); it != m_workers.end(); ++it ) (*it)->join();
		
		my_time_t tick_end;
		get_time ( tick_end );
		
		double* A_result = m_workers[0]->result();
		
		if ( debug & 4 ) {
			for ( size_t yy=0; yy<h; ++yy) {
				cerr << '[' << yy << "@END]\t";
				for ( size_t xx = 0; xx<w; ++xx ) {
					cerr << RESULT(yy, xx) << '\t';
				}
				cerr << endl;
			}
			cerr << " ================================ " << endl;
		}
		
#ifndef NDEBUG
		cerr << "[ThreadDriver] Compute result"<<A_result<<endl;
#endif // NDEBUG
		
		if (checkdata) checkdata->compare(ArrayPart<double>(A_result, w, h, 1, 1, w-2, h-2), "ThreadDriver");
		  
		delete[] A_from;
		delete[] A_to;
		
		return timing_diff_usec(tick_start, tick_end);
	}
	
};

LCMP(ThreadDriver)
	L_PROPERTY(uint8_t, debug);
	L_PROPERTY(uint16_t, nb_tiles_x);
	L_PROPERTY(uint16_t, nb_tiles_y);
	L_CPP_PROVIDE(XPComputer, xpcomputer);
	L_CPP_PROVIDE(LifeCycle, lifecycle);
	L_CPP_USE(CheckData, checkdata);
	L_CPP_USE(Computer, computers);
	L_CPP_USE(Initializer, initializations);
LEND

}

}

