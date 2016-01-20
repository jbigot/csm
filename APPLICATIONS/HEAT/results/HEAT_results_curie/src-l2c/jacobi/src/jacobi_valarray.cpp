#include<iostream>
#include<valarray>


#include <time.h>
#include <sys/time.h>
typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))


using namespace std;


const unsigned long niter = 1000;


int main()
{
	cout << "# datasize(bits, not bytes) time_one_iter(seconds)" << endl;
	
	for ( long w = 4; w < 15000 ; ) {
		
		if ( w < 2048 ) {
			w = w*2;
		} else {
			w = w + 1000;
		} 
		
		valarray<double> m_A(w*w);
		valarray<double> m_B(w*w);
		
		m_A[gslice(w+1, valarray<size_t>({w-2, w-2},2), valarray<size_t>({w, 1},2))] = -10; // init middle
		
		m_A[slice(0, w, w)] = 5; // init left
		m_A[slice(w-1, w, w)] = 5; // init right
		
		for ( i=0 ; i < w ; i++ ) {
			A(i, w-1) = 5;
			A(i, 0) = 5;
		}
		
		for ( i = 0 ; i < w ; i++ ) A(w-1, i) = 0;
		for ( i = 0 ; i < w ; i++ ) A(0, i) = 10;
		
		for ( i = 0 ; i < w ; i++ ) {
			for ( j = 0 ; j < w ; j++ ) {
				B(i, j) = A(i, j);
			}
		}
		
		my_time_t tick_start;
		get_time ( tick_start );
		
		for ( unsigned long iter = 0 ; iter < niter ; iter++) {
			for ( j = 1; j < w-1 ; j++) {
				for ( i = 1; i < w-1 ; i++) {
					B(i, j) = 0.25*( A(i-1, j) + A(i+1, j) + A(i, j-1) +A(i, j+1) );
				}
			}
			
			swap(m_A, m_B);
			
		}
		
		my_time_t tick_end;
		get_time ( tick_end );
		
		
		long long time_usec = timing_diff_usec ( tick_start, tick_end );
		cout << w*w*64 <<"\t"<< time_usec/1000000.0/niter << endl;
	}
}
