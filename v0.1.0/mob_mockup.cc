#include "mob_mockup.hh"

#include "example.hh"

using namespace std;
using namespace Mob;

namespace Time
{
 void Tick()
 {
  counter += fullTick;

  dayTime += fullTick;
  if(dayTime > fullDay) {dayTime-=fullDay;}
 }

 double counter = 0;
 double dayTime = 0;
}

bool operator ==(Position const & p1, Position const & p2)
{
 return Distance(p1,p2) == 0 ? true : false;
}
bool operator !=(Position const & p1, Position const & p2)
{
 return not (p1 == p2);
}

double Distance(Position const & p1, Position const & p2)
{
 auto dx = std::abs(p1.x-p2.x);
 auto dy = std::abs(p1.y-p2.y);

 return std::pow(std::pow(dx,2)+std::pow(dy,2),0.5);
}



Mob::Mob::Mob( Mob const & original )
: name(original.name)
, behaviour( original.behaviour )
, ai(*this, behaviour)
{}

Mob::Mob::Mob( Mob && original )
: name(move(original.name))
, behaviour(original.behaviour) // moving reference makes no sense
, ai(*this, behaviour) // if moved, it would still point to old Mob
{}

Mob::Mob::operator Position() const
{
 return position;
}
bool Mob::Mob::operator< ( Mob const & other ) const
{
 return name < other.name;
}

void Mob::Mob::Update()
{
 ai.Set();

 endurance -= Time::fullTick;
 endurance = endurance > 1 ? 1 : (endurance < 0 ? 0 : endurance);

 if( endurance < .2 and name == "dog" and GetTaskName() != "rest" )
 {
  Interupt(Task::Rest(*this));
 }

 if( Time::dayTime > 18 and name != "dog" and GetTaskName() != "sleep" )
 {
  Interupt(Task::Sleep(*this));
 }

 auto distance = Distance(*this, destination);

 if( distance != 0 )
 {
  auto dx = Time::fullTick*(destination.x - position.x)/distance;
  auto dy = Time::fullTick*(destination.y - position.y)/distance;

  position.x = std::abs(destination.x - position.x) < std::abs(dx) ? destination.x : position.x + dx;
  position.y = std::abs(destination.y - position.y) < std::abs(dy) ? destination.y : position.y + dy;
 }
}

void Mob::Mob::Interupt( AI::task_type task )
{
 ai.Push(task);
}

bool Mob::Mob::Is(trait t) const
{
 return traits.count(t) == 1 ? true : false;
}

std::string Mob::Mob::GetTaskName()
{
 return ai.GetTaskName();
}
std::string Mob::Mob::GetActionName()
{
 return ai.GetActionName();
}
