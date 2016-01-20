#include <iostream>
#include <string>
#include <vector>

#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/llcmmpi.h>
#include <mpi.h>
#include <stdint.h>

#include <checkdata.h>

namespace jacobi {

namespace impl {

using ::std::cerr;
using ::std::copy;
using ::std::endl;
using ::std::max;
using ::std::string;
using ::std::vector;

class MpiConcentrator:
		virtual public CheckData
{
public:
	CheckData* central_check;
	
	uint32_t tile_x; ///< x index of this tile in nbtiles
	uint32_t tile_y;  ///< y index of this tile in nbtiles
	
	MPI_Comm comm;
	
	MpiConcentrator ():
			central_check(0),
			comm(MPI_COMM_NULL)
	{}
	
	struct PartInfo {
		PartInfo( size_t width, size_t height, size_t x_pos, size_t y_pos ):
				width(width),
				height(height),
				x_pos(x_pos),
				y_pos(y_pos)
		{}
		size_t width;
		size_t height;
		size_t x_pos;
		size_t y_pos;
	};
	
	virtual void compare ( ArrayPart< double > data, string msg )
	{
		int rank;
		MPI_Comm_rank(comm, &rank);
#ifndef NDEBUG
			cerr << "[MpiConcentrator|"<<rank<<"] compare( data(w="<<data.memory_width<<", h="<<data.memory_height<<", x="<<data.global_x<<", y="<<data.global_y<<", lw="<<data.width<<", lh="<<data.width<<"), "<<msg<<")"<<endl;
#endif //NDEBUG
		
		if ( rank == 0 ) {
			int size;
			MPI_Comm_size(comm, &size);
			
			vector< PartInfo > parts;
			size_t total_width = 0;
			size_t total_height = 0;
			
			size_t basic_width = 0;
			size_t basic_height = 0;
			
			// get our own info
			parts.push_back(PartInfo(data.width, data.height, tile_x, tile_y));
			if ( parts.back().y_pos == 0 ) total_width += parts.back().width;
			if ( parts.back().x_pos == 0 ) total_height += parts.back().height;
			
			if ( parts.back().x_pos == 0 && parts.back().y_pos == 0 ) {
				basic_height = parts.back().height;
				basic_width = parts.back().width;
			}
			
			// get info from everyone
			for ( int other_rank = 1; other_rank < size; ++other_rank ) {
				unsigned long long infos[4];
				MPI_Status status;
				MPI_Recv(infos, 4, MPI_UNSIGNED_LONG_LONG, other_rank, 0, comm, &status);
				
				parts.push_back(PartInfo(infos[0], infos[1], infos[2], infos[3]));
				if ( parts.back().y_pos == 0 ) total_width += parts.back().width;
				if ( parts.back().x_pos == 0 ) total_height += parts.back().height;
			
				if ( parts.back().x_pos == 0 && parts.back().y_pos == 0 ) {
					basic_height = parts.back().height;
					basic_width = parts.back().width;
				}
			}
			
			double* full_matrix = new double[total_width*total_height];
			
			// get our own data
			for ( size_t yy = 0; yy < parts[0].height; ++yy ) {
				copy( &data[yy][0], &data[yy][data.width], full_matrix+(parts[0].y_pos*basic_height*total_width)+(yy*total_width)+(parts[0].x_pos*basic_width));
			}
			
			// get data from everyone
			for ( int other_rank = 1; other_rank < size; ++other_rank ) {
				MPI_Datatype subarray;
				MPI_Type_vector(parts[other_rank].height, parts[other_rank].width, total_width, MPI_DOUBLE, &subarray);
				MPI_Type_commit(&subarray);
				
				MPI_Status status;
				MPI_Recv(full_matrix+(parts[other_rank].y_pos*basic_height*total_width)+(parts[other_rank].x_pos*basic_width), 1, subarray, other_rank, 0, comm, &status);
				
				MPI_Type_free(&subarray);
			}
			
			central_check->compare(ArrayPart<double>(full_matrix, total_width, total_height, 0, 0, total_width, total_height), msg);

			delete[] full_matrix;
			
		} else {
			unsigned long long infos[4];
			
			infos[0] = data.width;
			infos[1] = data.height;
			infos[2] = tile_x;
			infos[3] = tile_y;
			
			MPI_Send(infos, 4, MPI_UNSIGNED_LONG_LONG, 0, 0, comm);
			
			MPI_Datatype subarray;
			MPI_Type_vector(data.height, data.width, data.memory_width, MPI_DOUBLE, &subarray);
			MPI_Type_commit(&subarray);
			
			MPI_Send(&data[0][0], 1, subarray, 0, 0, comm);
			
			MPI_Type_free(&subarray);
		}
	}
};

LCMP(MpiConcentrator)
	L_PROPERTY(uint32_t, tile_y);
	L_PROPERTY(uint32_t, tile_x);
	L_CPP_PROVIDE(CheckData, data_checker);
	L_MPI_COMM(comm);
	L_CPP_USE(CheckData, central_check);
LEND

}

}

