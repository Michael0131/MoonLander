/***********************************************************************
 * Source File:
 *    ANGLE
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#include "angle.h"
#include <math.h>  // for floor()
#include <cassert>
using namespace std;

/************************************
 * ANGLE : NORMALIZE
 ************************************/
double Angle::normalize(double radians)
{

   // Map between 0 and (2*M_PI)
   while (radians < 0.0 || radians >(2 * M_PI)) {
      if (radians > (2 * M_PI)) {
         radians -= (2 * M_PI);
      }
      else {
         radians += (2 * M_PI);
      }
   }

   // Alternative
   //radians = fmod(radians, 2*M_PI);
   //if (radians < 0)
   //      // Wrap negatives to positive range; make sure it is in [0, TWO_PI)
   //   radians += (2*M_PI);


   return radians;
}

double Angle::convertToDegrees(double radians)
{
   return normalize(radians) * (180.0 / M_PI);
}

double Angle::convertToRadians(double degrees)
{
   return normalize(degrees * (M_PI / 180.0));
}

