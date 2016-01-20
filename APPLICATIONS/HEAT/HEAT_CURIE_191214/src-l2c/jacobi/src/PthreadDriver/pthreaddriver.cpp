#include <iostream>
#include <vector>

#include <pthread.h>

#include <mybarrier.h> // Component LocalBarrier

#include <llcmcpp/lifecycle.h>
#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <algorithm>

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

using ::llcmcpp::LifeCycle;
using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::swap;
using ::std::vector;

namespace jacobi {

namespace impl {

class PthreadDriver :
		virtual public XPComputer,
		virtual public LifeCycle
{
public:
	uint8_t debug;
	
	uint16_t nb_tiles_x, nb_tiles_y;
	
	CheckData* checkdata2;
	
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
	
	threads::MyBarrier* mybarrier;  

	PthreadDriver ():
			debug(0xFF),
			nb_tiles_x(1),
			nb_tiles_y(1),
			checkdata2(0)
	{
	}
	
private:
	class Worker
	{
	public:
		Computer* computer;
		Initializer* initialization;

		threads::MyBarrier* mybarrier;  
		int id;
		
		pthread_mutex_t  m_mutex;
		pthread_cond_t m_cond;
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

		my_time_t tick_start, tick_end;

		Worker ( Computer* computer, Initializer* initialization, threads::MyBarrier* barrier, int id ):
				computer(computer),
				initialization(initialization),
				m_status(STOPPED),
				mybarrier(barrier),
				id(id)
		{
		  pthread_mutex_init(&m_mutex, NULL);
		  pthread_cond_init(&m_cond, NULL);
		}
		
		void init ( double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_x, const size_t tile_y, const size_t tile_width, const size_t tile_height, long niter )
		{
			pthread_mutex_lock(&m_mutex);
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
			pthread_cond_signal(&m_cond);
			pthread_mutex_unlock(&m_mutex);
		}
		
	  void prepare_to_run() {
			pthread_mutex_lock(&m_mutex);
			cerr << "Thread "<<this<<" ts="<<timing_usec(tick_start)<<" te="<<timing_usec(tick_end)<<" tt="<<timing_diff_usec(tick_start, tick_end)<<endl;
			while ( m_status != INITIALIZED ) 
			  pthread_cond_wait(&m_cond, &m_mutex);
			pthread_mutex_unlock(&m_mutex);
	  }

		void run ()
		{
			pthread_mutex_lock(&m_mutex);
			m_status = RUN;
			pthread_cond_signal(&m_cond);
			pthread_mutex_unlock(&m_mutex);
		}
		
	  static void* start_thread(void* arg) {
		 Worker* w = static_cast<Worker*>(arg);
		 return w->start();
	  }

	  void* start()
		{
			pthread_mutex_lock(&m_mutex);
			for (;;) {
				while ( m_status != INIT ) 
				  pthread_cond_wait(&m_cond, &m_mutex);

				initialization->init(
						ArrayPart< double >(m_A_from, m_height, m_width, m_tile_x, m_tile_y, m_tile_width, m_tile_height),
						m_tile_x==0,
						m_tile_x+m_tile_width==m_width,
						m_tile_y==0,
						m_tile_y+m_tile_height==m_height
				);
		
				// Heating caches !
				for ( int iter=0; iter<std::min(10L, m_niter); ++iter ) {
					computer->one_iter(m_A_from, m_A_to, m_width, m_height, m_tile_x, m_tile_y, m_tile_width, m_tile_height );
					swap(m_A_from, m_A_to);
					mybarrier->barrier();
				}

				//				cerr << "[Thread] iter="<<m_niter<<". m_width="<<m_width<<", m_height="<<m_height<<", m_tile_x="<<m_tile_x<<", m_tile_y="<<m_tile_y<<", m_tile_width="<<m_tile_width<<", m_tile_height="<<m_tile_height<<"\n";

				m_status = INITIALIZED;
				pthread_cond_signal(&m_cond);

				while ( m_status != RUN ) 
				  pthread_cond_wait(&m_cond, &m_mutex);

				get_time ( tick_start );

				for ( long iter=0; iter<m_niter; ++iter ) {
					computer->one_iter(m_A_from, m_A_to, m_width, m_height, m_tile_x, m_tile_y, m_tile_width, m_tile_height );
					swap(m_A_from, m_A_to);
					mybarrier->barrier();
				}
				get_time ( tick_end );

				m_status = STOPPED;
				pthread_cond_signal(&m_cond);

			}
			pthread_mutex_unlock(&m_mutex);
		}
		
		void join ()
		{
			pthread_mutex_lock(&m_mutex);
			while ( m_status != STOPPED ) 
			  pthread_cond_wait(&m_cond, &m_mutex);
			pthread_mutex_unlock(&m_mutex);
		}
		
		double* result ()
		{
			pthread_mutex_lock(&m_mutex);
			double* res=m_A_from;
			pthread_mutex_unlock(&m_mutex);
			return res;
		}
	};

public:
	vector< Worker* > m_workers;
	vector<pthread_t> m_threads;
	
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
		for ( int ii=0; ii<m_computers.size(); ++ii) {
			Worker* w = new Worker(m_computers[ii], m_initializations[ii], mybarrier, ii);
			pthread_t tid;
			pthread_create(&tid, NULL, Worker::start_thread, static_cast<void*>(w));
			m_threads.push_back(tid);
			m_workers.push_back(w);
		}
		m_computers.clear();
	}
	
	uint64_t compute(size_t w, size_t h , unsigned long niter)
	{
		if (debug & 8) cout << "[ThreadDriver] Compute!" << endl;
		
		double* A_from = new double[h*w];
		double* A_to = new double[h*w];
		
		size_t tile_height = (h-2)/nb_tiles_y;
		size_t tile_width = (w-2)/nb_tiles_x;
		
		vector< Worker* >::iterator it = m_workers.begin();
		for ( size_t tile_y = 0; tile_y < nb_tiles_y; ++ tile_y ) {
			for ( size_t tile_x = 0; tile_x < nb_tiles_x; ++ tile_x ) {
				size_t local_width = tile_width;
				if ( tile_x == nb_tiles_x-1 ) local_width = w-2 - tile_x*tile_width;
				size_t local_height = tile_height;
				if ( tile_y == nb_tiles_y-1 ) local_height = h-2 - tile_y*tile_height;
				(*it)->init(A_from, A_to, w, h, 1+tile_x*tile_width, 1+tile_y*tile_height, local_width, local_height, niter);
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
		
		if (checkdata2) checkdata2->compare(ArrayPart<double>(A_from, w, h, 1, 1, w-2, h-2), "PthreadDriver");
		  
		delete[] A_from;
		delete[] A_to;
		
		return timing_diff_usec(tick_start, tick_end);
	}
	
};

LCMP(PthreadDriver)
	L_PROPERTY(uint8_t, debug);
	L_PROPERTY(uint16_t, nb_tiles_x);
	L_PROPERTY(uint16_t, nb_tiles_y);
	L_CPP_PROVIDE(XPComputer, xpcomputer);
	L_CPP_PROVIDE(LifeCycle, lifecycle);
	L_CPP_USE(CheckData, checkdata2);
	L_CPP_USE(Computer, computers);
	L_CPP_USE(Initializer, initializations);
	L_CPP_USE(threads::MyBarrier, mybarrier);
LEND

}

}

