#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include<math.h>

//-------------------------------------------------------------------------------
//header structure of the MNT binary file
//-------------------------------------------------------------------------------
struct HEADER
//-------------------------------------------------------------------------------
{
  unsigned int width;
  unsigned int height;
  float x;//low left
  float y;//low left
  //float spacing;
  float dx;
  float dy;
  float nodata; //no data value (sparse information in input files)
};
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
struct Header
//-------------------------------------------------------------------------------
{
  static HEADER read(const char * binFile)
  {
    HEADER head;
    std::ifstream f(binFile, std::ios::binary | std::ios::in);
    f.read(reinterpret_cast<char*>(&head),sizeof(HEADER));
    
    return head;
  }
};
//------------------------------------------------------------------------------

#endif
