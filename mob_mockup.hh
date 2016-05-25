#pragma once

#include <cmath>
#include <set>

#include "AI.hh"

namespace Time
{
 void Tick();

 extern double counter;
 extern double dayTime; // in-game hours since sunrise


 double const fullTick = 60./(60.*60.); // one tick is equal 1 in-game minute
 double const fullDay = 24;
}

struct Position
{
 double x;
 double y;
};

bool operator ==(Position const & p1, Position const & p2);
bool operator !=(Position const & p1, Position const & p2);
double Distance(Position const & p1, Position const & p2);


namespace Mob
{
 enum class trait
 {
  lazy
 };

 class Mob
 {
  friend Mob NewFred();
  friend Mob NewBob();
  friend Mob NewDog();

  public:
   Mob( Mob const & );
   Mob( Mob && );

   std::string name {"no_name_specified"};

   void Update();
   void Interupt( AI::task_type );

   bool Is(trait) const;

   double endurance {1.};
   double hunger {1.};

   std::string GetTaskName();
   std::string GetActionName();


   operator Position() const;
   bool operator< ( Mob const & other ) const;

   Position destination {0,0};
   Position position {0,0};

  protected:
   Mob( AI::Behaviour behaviour )
   : behaviour( behaviour )
   , ai(*this, behaviour)
   {};

  private:
   std::set<trait> traits;

   AI::Behaviour behaviour;
   AI::AI ai;
 };

 Mob NewFred();
 Mob NewBob();
 Mob NewDog();
}
