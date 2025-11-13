// Developed by Group 7 - Brayden Jones, James Gassaway, Michael Johnson

/**********************************************************************
 * LAB 06
 * Lunar Lander simulation. This is the Game class and main()
 **********************************************************************/

#include "position.h"    // everything should have a point
#include "lander.h"      // the lander
#include "star.h"        // a star
#include "acceleration.h" // for acceleration
#include "angle.h"       // angle of the lander
#include "uiInteract.h"  // for INTERFACE
#include "uiDraw.h"      // for RANDOM and DRAW*
#include "ground.h"      // for GROUND
#include "test.h"        // for the unit tests
#include <cmath>         // for SQRT
#include <cassert>       // for ASSERT
#include <vector>        // for vector of Stars
#include <iomanip>       // for text precision
using namespace std;


/*************************************************************************
 * SIMULATOR
 * Everything pertaining to the simulator.
 *************************************************************************/
class Simulator
{
public:
   // set up the simulator
   Simulator(const Position& posUpperRight) : ground(posUpperRight), lander(posUpperRight), stars(), thrust(), acceleration() {}

   // display stuff on the screen
   void display();

   Lander lander;

   vector<Star> stars;

   Thrust thrust;
   Ground ground;

   Acceleration acceleration;

   const double gravity = -1.625; // Moon gravity: -1.625 (m/s^2)

};

/**********************************************************
 * DISPLAY
 * Draw on the screen
 **********************************************************/
void Simulator::display()
{
   ogstream gout;

   // draw a star
   for (Star& star : stars)
   {
      star.draw(gout);
   }
      
   // draw the ground
   ground.draw(gout);

   // draw the lander
   lander.draw(thrust, gout);
    
   // -- Text --
   // Draw the text
   Position fuelMetric = Position(15.0, 375.0);
   Position altitudeMetric = Position(15.0, 350.0);
   Position speedMetric = Position(15.0, 325.0);

   // Finishing message
   Position finishMessage = Position(150.0, 300.0);
   gout.setPosition(finishMessage);  // Set top-left corner position
   
   // Live metrics
   if (lander.isFlying())
   {
      // Fuel
      gout.setPosition(fuelMetric);
      gout << "Fuel: " << lander.getFuel() << " lbs";

      // Altitude
      gout.setPosition(altitudeMetric);
      gout << "Altitude: " << int(lander.getPosition().getY()) << " meters";

      // Speed
      gout.setPosition(speedMetric);
      gout << fixed << setprecision(2);
      gout << "Speed: " << lander.getSpeed() << " m/s";
   }

   // Finish - Crash or Land
   else if (lander.isDead())
   {

      gout << "Houston, we have a problem!";   // Write text to the buffer
   }

   else if (lander.isLanded())
   {
      gout << "The Eagle has landed!";
   }
   
   gout.flush();           // Force it to draw
      
}


/*************************************
 * CALLBACK
 * Handle one frame of the simulator
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL. 
   Simulator* pSimulator = (Simulator*)p;


   if (pSimulator->lander.isFlying())
   {
      // Update thrust settings based on user input
      pSimulator->thrust.set(pUI);

      // Apply thrust input and compute acceleration
      pSimulator->acceleration = pSimulator->lander.input(pSimulator->thrust, pSimulator->gravity);

      // Update lander's position and velocity
      pSimulator->lander.coast(pSimulator->acceleration, /* time step */ (0.1));


      // Check for collision with the ground
      if (pSimulator->ground.hitGround(pSimulator->lander.getPosition(), pSimulator->lander.getWidth()))
      {
         pSimulator->lander.crash();
      }

      // Check for successful landing on the landing platform
      if (pSimulator->ground.onPlatform(pSimulator->lander.getPosition(), pSimulator->lander.getWidth()))
      {
         if (pSimulator->lander.getSpeed() > pSimulator->lander.getMaxSpeed())
         {
            pSimulator->lander.land();
         }

         else {
            // Landed too quickly, so we crash
            pSimulator->lander.crash();
         }
            
      }
   }
   
   // Draw everything
   pSimulator->display();

}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my LM type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Run the unit tests
   testRunner();


   // Initialize OpenGL
   Position posUpperRight(400, 400);
   Interface ui("Lunar Lander", posUpperRight);

   // Initialize the game class
   Simulator simulator(posUpperRight);

   simulator.lander.reset(posUpperRight);

   
   for (int i = 0; i < 50; ++i)
   {
      Star star;
      star.reset(posUpperRight.getX(), posUpperRight.getY());
      simulator.stars.push_back(star);
   }
   
   // set everything into action
   ui.run(callBack, (void*)&simulator);

   return 0;
}
