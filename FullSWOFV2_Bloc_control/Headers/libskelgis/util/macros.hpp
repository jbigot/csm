#ifndef MACROS_H
#define MACROS_H

#include "mpi.h"
#include "dmatrix_vector.hpp"

namespace skelgis{

//------------MPI FUNCS
#define MPI_RANK_NB MPI_Comm_size(MPI_COMM_WORLD,&(Mpi_::mpi_nb));	\
MPI_Comm_rank(MPI_COMM_WORLD,&(Mpi_::mpi_rank));
#define MPI_END MPI_Finalize();
//------------
//------------SKELGIS INIT
#define INITSKELGIS MPI_Init(&argc,&argv); \
MPI_RANK_NB
//-----------
//-----------SKELGIS END
#define ENDSKELGIS MPI_END
//---------- 

//-----------SKELETON APPLYONE FUNCTION
#define BEGINApplyUnary(name, input_matrix, input_type, r, output_matrix, output_type, r2) struct _ApplyUnary_##name : public _ApplyUnary_Func<input_type, r, output_type,r2> \
{ \
  void operator()(DMatrix<input_type,r>& input_matrix, DMatrix<output_type,r2>& output_matrix) const
//-----------
//-----------SKELETON APPLYONE FUNCTION  - BLOCK DIVISION
#define BEGINApplyUnaryBlock(name, input_matrix, input_type, r, output_matrix, output_type, r2) struct _ApplyUnary_##name : public _ApplyUnary_Func<input_type,r, output_type,r2,false> \
{ \
  void operator()(DMatrix<input_type,r,false>& input_matrix, DMatrix<output_type,r2,false>& output_matrix) const
//-----------

//-----------SKELETON APPLYONEWITH FUNCTION
#define BEGINApplyUnaryWith(name, input_matrix, input_type, r, output_matrix, output_type, r2, object) struct _ApplyUnaryWith_##name : public _ApplyUnaryWith_Func<input_type,r, output_type,r2> \
{ \
  void operator()(DMatrix<input_type,r>& input_matrix, DMatrix<output_type,r2>& output_matrix, void * object) const
//-----------
//-----------SKELETON APPLYONEWITH FUNCTION - BLOCK DIVISION
#define BEGINApplyUnaryWithBlock(name, input_matrix, input_type, r, output_matrix, output_type, r2, object) struct _ApplyUnaryWith_##name : public _ApplyUnaryWith_Func<input_type,r, output_type,r2,false> \
{ \
  void operator()(DMatrix<input_type,r,false>& input_matrix, DMatrix<output_type,r2,false>& output_matrix, void * object) const
//-----------

//-----------SKELETON APPLYTWO FUNCTION
#define BEGINApplyBinary(name, input_matrix1, input_type1, input_matrix2, input_type2,r, output_matrix, output_type,r3) struct _ApplyBinary_##name : public _ApplyBinary_Func<input_type1,r,input_type2,output_type,r3> \
{ \
  void operator()(DMatrix<input_type1,r>& input_matrix1,DMatrix<input_type2,r>& input_matrix2, DMatrix<output_type,r3>& output_matrix) const
//-----------
//-----------SKELETON APPLYTWO FUNCTION - BLOCK DIVISION
#define BEGINApplyBinaryBlock(name, input_matrix1, input_type1, input_matrix2, input_type2,r, output_matrix, output_type,r3) struct _ApplyBinary_##name : public _ApplyBinary_Func<input_type1,r,input_type2,output_type,r3,false> \
{ \
  void operator()(DMatrix<input_type1,r,false>& input_matrix1,DMatrix<input_type2,r,false>& input_matrix2, DMatrix<output_type,r3,false>& output_matrix) const
//-----------

//-----------SKELETON APPLYTWOWITH FUNCTION
#define BEGINApplyBinaryWith(name, input_matrix1, input_type1, input_matrix2, input_type2,r, output_matrix, output_type,r2, object) struct _ApplyBinaryWith_##name : public _ApplyBinaryWith_Func<input_type1,r,input_type2, output_type,r2> \
{ \
  void operator()(DMatrix<input_type1,r>& input_matrix1,DMatrix<input_type2,r>& input_matrix2,DMatrix<output_type,r2>& output_matrix, void * object) const
//-----------
//-----------SKELETON APPLYTWOWITH FUNCTION - BLOCK DIVISION
#define BEGINApplyBinaryWithBlock(name, input_matrix1, input_type1, input_matrix2, input_type2,r, output_matrix, output_type,r2, object) struct _ApplyBinaryWith_##name : public _ApplyBinaryWith_Func<input_type1,r,input_type2, output_type,r2,false> \
{ \
  void operator()(DMatrix<input_type1,r,false>& input_matrix1,DMatrix<input_type2,r,false>& input_matrix2,DMatrix<output_type,r2,false>& output_matrix, void * object) const
//-----------

//-----------SKELETON APPLYLIST FUNCTION
#define BEGINApplyList(name, inputs, inputs_type,r, outputs, outputs_type,r2) struct _ApplyList_##name : public _ApplyList_Func<inputs_type,r,outputs_type,r2> \
{ \
  void operator()(DMatrix_vector<inputs_type,r>& inputs, DMatrix_vector<outputs_type,r2>& outputs) const
//-----------
//-----------SKELETON APPLYLIST FUNCTION - BLOCK DIVISION
#define BEGINApplyListBlock(name, inputs, inputs_type,r, outputs, outputs_type,r2) struct _ApplyList_##name : public _ApplyList_Func<inputs_type,r, outputs_type,r2,false> \
{ \
  void operator()(DMatrix_vector<inputs_type,r,false>& inputs, DMatrix_vector<outputs_type,r2,false>& outputs) const
//-----------

//-----------SKELETON APPLYLISTWITH FUNCTION
#define BEGINApplyListWith(name, inputs, inputs_type,r, outputs, outputs_type,r2,object) struct _ApplyListWith_##name : public _ApplyListWith_Func<inputs_type,r,outputs_type,r2> \
{ \
void operator()(DMatrix_vector<inputs_type,r>& inputs, DMatrix_vector<outputs_type,r2>& outputs,void * object) const
//-----------
//-----------SKELETON APPLYLISTWITH FUNCTION - BLOCK DIVISION
#define BEGINApplyListWithBlock(name,inputs,inputs_type,r,outputs,outputs_type,r2,object) struct _ApplyListWith_##name : public _ApplyListWith_Func<inputs_type,r,outputs_type,r2,false> \
{ \
void operator()(DMatrix_vector<inputs_type,r,false>& inputs, DMatrix_vector<outputs_type,r2,false>& outputs,void * object) const
//-----------

//-----------END oF SKELETON FUNCTIONS
#define END(name)  }name;
//-----------


}

#endif
