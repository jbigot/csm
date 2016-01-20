#include <llcmcpp/llcmcpp3.h>

#include <arraypart.h>
#include <initializer.h>

namespace jacobi {

namespace impl {

class DataInitializer:
		virtual public Initializer
{
public:
	
	double top, bottom, left, right, inside;
	
	DataInitializer ():
			top(10),
			bottom(0),
			left(5),
			right(5),
			inside(-10)
	{}
	
	virtual void init ( ArrayPart< double > data, bool on_left, bool on_right, bool on_bottom, bool on_top )
	{
		// Middle Init
		for ( size_t yy=0; yy<data.height; ++yy ) {
			for ( size_t xx=0; xx<data.width; ++xx ) {
				data[yy][xx] = inside;
			}
		}
		
		// Left border
		if ( on_left ) {
			for ( size_t yy=0; yy<data.height; ++yy ) {
				data[yy][0] = left;
			}
		}
		
		// Right border
		if ( on_right ) {
			for ( size_t yy=0; yy<data.height; ++yy ) {
				data[yy][data.width-1] = right;
			}
		}
		
		// Bottom border
		if ( on_bottom ) {
			for ( size_t xx=0; xx<data.width; ++xx ) {
				data[0][xx] = bottom;
			}
		}
		
		// Top border
		if ( on_top ) {
			for ( size_t xx=0; xx<data.width; ++xx ) {
				data[data.height-1][xx] = top;
			}
		}
		
		// Bottom/Left corner
		if ( on_bottom && on_left ) {
			data[0][0] = (left+bottom)/2;
		}
		
		// Bottom/Right corner
		if ( on_bottom && on_right ) {
			data[0][data.width-1] = (right+bottom)/2;
		}
		
		// Top/Left corners
		if ( on_top && on_left ) {
			data[data.height-1][0] = (left+top)/2;
		}
		
		// Top/Righ corners
		if ( on_top && on_right ) {
			data[data.height-1][data.width-1] = (right+top)/2;
		}
	}
	
};

LCMP(DataInitializer)
	L_CPP_PROVIDE(Initializer, initializer);
	L_PROPERTY(double, top);
	L_PROPERTY(double, bottom);
	L_PROPERTY(double, left);
	L_PROPERTY(double, right);
	L_PROPERTY(double, inside);
LEND

}

}

