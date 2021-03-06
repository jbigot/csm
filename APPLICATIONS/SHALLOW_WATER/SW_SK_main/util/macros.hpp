#ifndef MACROS_H
#define MACROS_H

#include "mpi.h"
#include "../skeletons/skeletons.hpp"
#include "mpi_.hpp"

namespace skelgis{

//------------MPI FUNCS
#define MPI_RANK_NB MPI_Comm_size(MPI_COMM_WORLD,&(skelgis::Mpi_::mpi_nb)); \
  MPI_Comm_rank(MPI_COMM_WORLD,&(skelgis::Mpi_::mpi_rank));
#define MPI_END MPI_Finalize();
//------------
//------------SKELGIS INIT
#define INITSKELGIS MPI_Init(&argc,&argv); \
MPI_RANK_NB
//-----------
//-----------SKELGIS END
#define ENDSKELGIS MPI_END
//---------- 

//-----------SKELETON APPLYONE FUNCTION  - BLOCK DIVISION
#define BEGINApplyUnaryBlock(name, input_matrix, output_matrix) struct _ApplyUnary_##name : public skelgis::_ApplyUnary_Func \
{ \
void operator()(Data* input_matrix, Data* output_matrix) const
  //-----------

//-----------SKELETON APPLYONEWITH FUNCTION - BLOCK DIVISION
#define BEGINApplyUnaryWithBlock(name, input_matrix, output_matrix, object) struct _ApplyUnaryWith_##name : public skelgis::_ApplyUnaryWith_Func \
{ \
  void operator()(Data* input_matrix, Data* output_matrix, void * object) const
//-----------

//-----------SKELETON APPLYTWO FUNCTION - BLOCK DIVISION
#define BEGINApplyBinaryBlock(name, input_matrix1, input_matrix2, output_matrix) struct _ApplyBinary_##name :public skelgis::_ApplyBinary_Func \
{ \
  void operator()(Data* input_matrix1,Data* input_matrix2, Data* output_matrix) const
//-----------

//-----------SKELETON APPLYTWOWITH FUNCTION - BLOCK DIVISION
#define BEGINApplyBinaryWithBlock(name, input_matrix1, input_matrix2, output_matrix, object) struct _ApplyBinaryWith_##name : public skelgis::_ApplyBinaryWith_Func \
{ \
  void operator()(Data* input_matrix1,Data* input_matrix2,Data* output_matrix, void * object) const
//-----------

//-----------SKELETON APPLYLIST FUNCTION - BLOCK DIVISION
#define BEGINApplyListBlock(name, inputs, outputs) struct _ApplyList_##name : public skelgis::_ApplyList_Func \
{ \
  void operator()(std::vector<Data *>& inputs, std::vector<Data *>& outputs) const
//-----------

//-----------SKELETON APPLYLISTWITH FUNCTION - BLOCK DIVISION
#define BEGINApplyListWithBlock(name,inputs,outputs,object) struct _ApplyListWith_##name : public skelgis::_ApplyListWith_Func \
{ \
void operator()(std::vector<Data *>& inputs, std::vector<Data *>& outputs,void * object) const
//-----------

//-----------END oF SKELETON FUNCTIONS
#define END(name)  }name;
//-----------

}

#endif
