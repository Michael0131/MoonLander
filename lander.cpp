/***********************************************************************
 * Source File:
 *    LANDER
 * Author:
 *    Br. Helfrich
 * Summary:
 *    All the information about the lunar lander
 ************************************************************************/

#include "lander.h"
#include "acceleration.h"


/***************************************************************
 * RESET
 * Reset the lander and its position to start the game over
 ***************************************************************/
void Lander::reset(const Position& posUpperRight)
{
   // straight up
   angle.setUp();
   
   // the velocity is random
   velocity.setDX(random(-10.0, -4.0));
   velocity.setDY(random(-2.0, 2.0));

   // the position is at the right side of the screen
   pos.setX(posUpperRight.getX() - 1.0);
   pos.setY(random((0.75 * posUpperRight.getY()), (0.95 * posUpperRight.getY())));

   // status is playing
   status = PLAYING;

   // fill 'er up with fuel
   fuel = FUEL_MAX;

}

/***************************************************************
 * DRAW
 * Draw the lander on the screen
 ***************************************************************/
void Lander::draw(const Thrust& thrust, ogstream& gout) const
{
   // draw the lander on the screen
   gout.drawLander(pos, angle.getRadians());

   
   if (isFlying() && fuel > 0.0)
   {
      gout.drawLanderFlames(pos, angle.getRadians(), thrust.isMain(), thrust.isClock(), thrust.isCounter());
   }

   // OLD - This used Degrees instead of Radians
   //gout.drawLanderFlames(pos, angle.getDegrees(), thrust.isMain(), thrust.isClock(), thrust.isCounter());

}

/***************************************************************
 * INPUT
 * Accept input from the Neil Armstrong
 ***************************************************************/
Acceleration Lander::input(const Thrust& thrust, double gravity)
{

   // Set initial ddx (nothing) and ddy (gravity)
   double ddx = 0.0;
   double ddy = gravity;

   // - Acceleration -
   // Check if the thruster is activating and there is fuel
   if (thrust.isMain() && fuel > 0.0)
   {
      // Add acceleration in accordance to the angle and thrust of main engine
      ddx += -sin(angle.getRadians()) * thrust.mainEngineThrust();
      ddy += cos(angle.getRadians()) * thrust.mainEngineThrust();

      // Spend a chunk of fuel
      fuel -= 10.0;
   }

   // - Angle - 
   // Check if either side thrusters are activating
   if (thrust.rotation() && fuel > 0.0)
   {
      // Adjust the angle in accordance to the rotation
      angle.setRadians(angle.getRadians() + thrust.rotation());

      // Spend a little bit of fuel
      fuel -= 1.0;
   }

   if (fuel < 0.0)
   {
      fuel = 0.0;
   }

   // Return the ddx and ddy as an Acceleration object
   return Acceleration(ddx, ddy);
   
}

/******************************************************************
 * COAST
 * What happens when we coast?
 *******************************************************************/
void Lander :: coast(Acceleration & acceleration, double time)
{

   // Check if there is time
   if (time > 0)
   {
      // update the position
      pos.add(acceleration, velocity, time);

      // update the velocity
      velocity.add(acceleration, time);

   }
}
