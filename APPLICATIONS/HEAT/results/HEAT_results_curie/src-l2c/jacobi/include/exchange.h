#ifndef JACOBI_EXCHANGE_H
#define JACOBI_EXCHANGE_H


namespace jacobi {

class Exchange
{
public:
	virtual void send_ready( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height)=0;
	virtual void recv_wait()=0;
	virtual void send_wait()=0;
	virtual void recv_ready( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height)=0;
};

}

#endif // JACOBI_EXCHANGE_H
