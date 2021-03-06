#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include<math.h>

#include "../data_structures/dmatrix.hpp"

//namespace skelgis{

  //-------------------------------------------------------------------------------
  //header structure of the MNT binary file
  //-------------------------------------------------------------------------------
  struct HEADER
  //-------------------------------------------------------------------------------
  {
    unsigned int width;
    unsigned int height;
    float x;
    float y;
    float spacing;
    float nodata;
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

      /*int w = floor(head.width/20);
	head.width=w;
	int h = floor(head.height/20);
	head.height=h;*/

      return head;
    }
  };
  //------------------------------------------------------------------------------
//}

#endif
