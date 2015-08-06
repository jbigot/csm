#ifndef MPIOMP_H
#define MPIOMP_H

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "mpi.h"

namespace skelgis{

  //!  mpi Class 
  /*!
    This template class defines the MPI initializations, the MPI communications
  */
  //-------------------------------------------------------------------------------
  class Mpi_
  //-------------------------------------------------------------------------------
  {
  public:
    //--------------------------------------
    //VARIABLES
    //--------------------------------------
    static int level_provided;
    static int mpi_rank;
    static int mpi_nb;
    //--------------------------------------	

    //! Constructor
    /*!
     */
    //-------------------------------------------------------------------------------
    Mpi_(){}
    //-------------------------------------------------------------------------------

    //! Destructor
    /*!
     */
    //-------------------------------------------------------------------------------
    ~Mpi_(){}
    //-------------------------------------------------------------------------------
    //! Print the string str in output files
    /*!
      Only used for verifications because of synchronizations time
      \param str is the string to print
    */
    //-------------------------------------------------------------------------------
    static void print(std::string str)
    //-------------------------------------------------------------------------------
    {
      std::stringstream st2;
      st2<<"\nMPI "<<Mpi_::mpi_rank<<" - "<<" : "<<str;
   
      mkdir("outputs", 01777);
      std::stringstream fileStr;
      fileStr<<"./outputs/log_mach"<<Mpi_::mpi_rank<<".txt";
      std::ofstream fileW(fileStr.str().data(),std::ios_base::app);
      fileW.write(st2.str().data(),st2.str().size());
      fileW.close();
    }

    //! Print the string str on screen
    /*!
      Only used for verifications because of synchronizations time
      \param str is the string to print
    */
    //-------------------------------------------------------------------------------
    static void printScreen(std::string str)
    //-------------------------------------------------------------------------------
    {
      std::stringstream st2;
      st2<<"MPI "<<Mpi_::mpi_rank<<" - "<<" : "<<str;
      std::cout<<st2.str()<<std::endl;
    }
  };

  //-------------------------------------------------------------------------------
  //Instances
  //-------------------------------------------------------------------------------
  /*int Mpi_::level_provided=0;
  int Mpi_::mpi_rank=0;
  int Mpi_::mpi_nb=0;*/

}

#endif

