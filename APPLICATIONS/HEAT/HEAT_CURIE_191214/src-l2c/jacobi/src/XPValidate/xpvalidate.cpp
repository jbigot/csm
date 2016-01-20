#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp3.h>

#include <checkdata.h>
#include <XPcomputer.h>

using ::std::abs;
using ::std::cerr;
using ::std::copy;
using ::std::cout;
using ::std::endl;
using ::std::max;
using ::std::pair;
using ::std::string;
using ::std::vector;

namespace jacobi {

namespace impl {

class XPValidate :
			virtual public CheckData,
			virtual public llcmcpp::Go
{

public:
	
	void computers ( XPComputer* computer )
	{
		m_computers.push_back(computer);
	}
	vector< XPComputer* > m_computers;
	
	uint8_t verbosity;
	
	XPValidate ():
			verbosity(0),
			m_matrix(0)
	{
	}
	
	~XPValidate ()
	{
		delete[] m_matrix;
	}
	
	virtual void compare ( ArrayPart< double > data, string msg )
	{
		if ( data.width < 10 && data.height < 20 ) {
			for ( int yy=data.height-1; yy>=0; --yy) {
				cout << '['<<yy<<"/"<<data.height-1<<"|"<<msg<<"]\t";
				for ( size_t xx = 0; xx<data.width; ++xx ) {
					cout << data(yy,xx) << '\t';
				}
				cout << endl;
			}
		}
		if ( !m_matrix ) {
			m_matrix = new double[data.width*data.height];
			for ( size_t yy = 0; yy<data.height; ++yy ) {
				copy(&data[yy][0], &data[yy][data.width], m_matrix+yy*data.width);
			}
			m_width = data.width;
			m_height = data.height;
			m_msg = msg;
		} else {
			if ( data.height != m_height ) {
				cerr << "different heights: "<<data.height<<"("<<msg<<") and "<<m_height<<"("<<m_msg<<")"<<endl;
				abort();
			}
			if ( data.width != m_width ) {
				cerr << "different widths: "<<data.width<<"("<<msg<<") and "<<m_width<<"("<<m_msg<<")"<<endl;
				abort();
			}
			double max_error = 0;
			for ( int yy=0; yy<data.height; ++yy) {
				for ( size_t xx = 0; xx<data.width; ++xx ) {
					max_error = max( abs(data[yy][xx] - m_matrix[yy*m_width+xx]), max_error);
				}
			}
			cout << "max error between "<<m_msg<<" and "<<msg<<": "<<max_error<<endl;
		}
		cout << endl;
	}
	
	struct Test
	{
		Test ( unsigned long niter, size_t xx, size_t yy ):
				niter(niter),
				xx(xx),
				yy(yy)
		{}
		unsigned long niter;
		size_t xx;
		size_t yy;
	};
	
	static const Test tested[];
	
	static const size_t tested_size;
	
	virtual void go ()
	{
		cout << "[XPValidate] go!" << endl;
		
		for ( int ii=0; ii<tested_size; ++ii ) {
			if (verbosity & 1 ) cout << ii<<"/"<<tested_size<<":height(nbdouble): "<<tested[ii].yy<<"\t width(nbdouble): "<<tested[ii].xx<<"\t niter: "<<tested[ii].niter<<"\n"<<endl;
			for ( vector< XPComputer* >::const_iterator it = m_computers.begin(); it != m_computers.end(); ++it ) {
				(*it)->compute(tested[ii].xx, tested[ii].yy, tested[ii].niter);
			}
			delete[] m_matrix;
			m_matrix = 0;
		}

	}

private:
	double* m_matrix;
	
	size_t m_width, m_height;
	
	string m_msg;
	
};

const XPValidate::Test XPValidate::tested[] = {
	XPValidate::Test(1234, 4, 4),
	XPValidate::Test(0, 4, 4),
	XPValidate::Test(2, 4, 4),
	XPValidate::Test(2, 8, 8),
	XPValidate::Test(100, 4, 7),
	XPValidate::Test(112, 10, 5),
	XPValidate::Test(100, 25, 27),
	XPValidate::Test(40, 16, 100),
	XPValidate::Test(22, 100, 16),
	XPValidate::Test(2, 1024, 1024)
};

const size_t XPValidate::tested_size = sizeof(tested)/sizeof(XPValidate::Test);

LCMP(XPValidate)
	L_PROPERTY(uint8_t, verbosity);
	L_CPP_PROVIDE(llcmcpp::Go, go);
	L_CPP_PROVIDE(CheckData, checkdata);
	L_CPP_USE(XPComputer, computers);
LEND

}

}
