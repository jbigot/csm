#ifndef JACOBI_ARRAYPART_H
#define JACOBI_ARRAYPART_H

#include <cstring> // size_t

namespace jacobi {

template < typename D >
class ArrayPart
{
public:
	/**
	 * \param data the matrix
	 * \param memory_width width of the matrix (memory)
	 * \param memory_height height of the matrix (memory)
	 * \param tile_x position of the tile accesses by this ArrayPart in the matrix (x)
	 * \param tile_y position of the tile accesses by this ArrayPart in the matrix (y)
	 * \param tile_width width of the tile accesses by this ArrayPart
	 * \param tile_height height of the tile accesses by this ArrayPart
	 */
	ArrayPart ( D* data, size_t memory_width, size_t memory_height, size_t tile_x, size_t tile_y, size_t tile_width, size_t tile_height ):
			data(data),
			memory_width(memory_width),
			memory_height(memory_height),
			global_x(tile_x),
			global_y(tile_y),
			width(tile_width),
			height(tile_height)
	{}
	
	class Row
	{
	public:
		Row ( ArrayPart<D>& array, size_t y ):
				array(array),
				y(y)
		{}
		
		D& operator[] ( size_t x )
		{
			return array(y,x);
		}
		
		ArrayPart<D>& array;
		
		size_t y;
	};
	
	Row operator[] ( size_t y )
	{
		return Row(*this, y);
	}
	
	D& operator() ( size_t y, size_t x )
	{
		return data[(global_y+y)*memory_width+(global_x+x)];
	}
	
	D* data;
	
	size_t memory_width;
	
	size_t memory_height;
	
	size_t global_x;
	
	size_t global_y;
	
	size_t width;
	
	size_t height;
	
};

}

#endif // JACOBI_ARRAYPART_H
