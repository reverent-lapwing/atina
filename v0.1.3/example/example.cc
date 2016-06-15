#include <iostream>
#include <map>

#include "mob_mockup.hh"
#include "example.hh"

using namespace atina;
using namespace std;


map<string, Position> town
{
 {"Fred's House", {.2,.3}},
 {"Bob's House", {.4,.1}},
 {"Farm", {1.,-3.2}}
};



namespace Actions
{
 class Wait : public action_base
 {
  public:
   static Action New( double toWait )
   {
    return {make_unique<Wait>(Wait{toWait})};
   }
   Action Copy()
   {
    return New(toWait);
   }

   status update( Mob::Mob & mob )
   {
    elapsed += Time::fullTick;
    mob.endurance += 2*Time::fullTick;

    return elapsed < toWait ? status::not_finished : status::finished;
   }

  private:
   Wait( double toWait )
   : action_base("wait")
   , toWait(toWait)
   , elapsed(0)
   {}

   double elapsed;
   double const toWait;
 };

 class GoTo : public action_base
 {
  public:
   static Action New( Position destination )
   {
    return {make_unique<GoTo>(GoTo{destination})};
   }
   Action Copy()
   {
    return New(destination);
   }

   status update( atina::mob_type & mob )
   {
    mob.destination = destination;

    return mob.position != destination ? status::not_finished : status::finished;
   }

  private:
   GoTo( Position destination )
   : action_base("go_to")
   , destination(destination)
   {}

   Position const destination;
 };

 class Interupt : public action_base
 {
  public:
   static Action New( function<Task(Mob::Mob&)> task )
   {
    return {make_unique<Interupt>(Interupt{task})};
   }
   Action Copy()
   {
    return New(task);
   }

   status update( Mob::Mob & mob )
   {
    mob.Interupt(task(mob));

    return status::finished;
   }

  private:
   Interupt( function<Task(Mob::Mob&)> task )
   : action_base("interupt")
   , task(task)
   {}

   function<Task(Mob::Mob&)> task;
 };
}

namespace Tasks
{
 decision_tree_type roamDecisionTree
 {
  {{1,2},{[](Mob::Mob const & mob){ return mob.position == town.at("Farm") ? true : false; },{}}},
  {{3},{[](Mob::Mob const & mob){ return true; },{Actions::Interupt::New(Rest)}}},
  {{1,3},{[](Mob::Mob const & mob){ return (mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.7 : 0.5)) ? true : false; },{Actions::GoTo::New(town.at("Farm"))}}},
  {{4,5},{[](Mob::Mob const & mob){ return mob.name == "bob" ? true : false; },{}}},
  {{-1},{[](Mob::Mob const & mob){ return true; },{Actions::GoTo::New(town.at("Bob's House")),Actions::Interupt::New(Rest)}}},
  {{-1, 3},{[](Mob::Mob const & mob){ return (mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.7 : 0.5)) ? true : false; },{Actions::GoTo::New(town.at("Fred's House")),Actions::GoTo::New(town.at("Bob's House")),Actions::GoTo::New(town.at("Farm"))}}}
 };

 Task Sleep( Mob::Mob & mob ){return {"sleep", mob,
 { mob.name == "dog" ? Action::None() : Actions::GoTo::New( mob.name == "fred" ? town.at("Fred's House") : town.at("Bob's House") )
 , Actions::Wait::New(mob.Is(Mob::trait::lazy) ? 10 : 8)}};};
 Task Eat(Mob::Mob & mob){return {"eat", mob, {}};};
 Task Rest(Mob::Mob & mob){return {"rest", mob, {Actions::Wait::New(mob.Is(Mob::trait::lazy) ? 2 : 1)}};};
 Task Roam(Mob::Mob & mob){return {"roam", mob, roamDecisionTree}; cout << "roam_push"; };
 Task Cry(Mob::Mob & mob){return {"cry", mob, {}};};
}

namespace AIs
{
 Task NPC(Mob::Mob & mob)
 {
  if( mob.hunger < (mob.Is(Mob::trait::lazy) ? 0.2 : 0.5))
  {
   return Tasks::Eat(mob);
  }

  if( Time::dayTime > 16)
  {
   return Tasks::Sleep(mob);
  }

  if( mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.5 : 0.2))
  {
   return mob.Is(Mob::trait::lazy) ? Tasks::Sleep(mob) : Tasks::Rest(mob);
  }

  return Tasks::Roam(mob);
 };

 Task Pet(Mob::Mob & mob)
 {
  if( mob.hunger < 0.5)
  {
   return (mob.hunger < 0.2 ? Tasks::Eat(mob) : (mob.Is(Mob::trait::lazy) ? Tasks::Cry(mob) : Tasks::Eat(mob)));
  }

  if( mob.endurance == 1 )
  {
   return Tasks::Roam(mob);
  }

  return Tasks::Rest(mob);
 };
}


Mob::Mob Mob::NewFred()
{
 Mob fred(AIs::NPC);

 fred.name = "fred";

 return fred;
}

Mob::Mob Mob::NewBob()
{
 Mob bob{AIs::NPC};

 bob.name = "bob";

 return bob;
}

Mob::Mob Mob::NewDog()
{
 Mob dog{AIs::Pet};

 dog.name = "dog";
 dog.position.x = 9;
 dog.traits.insert(trait::lazy);

 return dog;
}


int main()
{
 deque<Mob::Mob> mobs;
 mobs.push_back(Mob::NewDog());
 mobs.push_back(Mob::NewFred());
 mobs.push_back(Mob::NewBob());


 while(true)
 {
  Time::Tick();
  cout<<"Time: "<<Time::dayTime<<"\n";

  for( auto & a : mobs )
  {
   cout<<"\n";
   a.Update();
   cout<<a.name<<"\n";
   cout<<"Task name: "<<a.GetTaskName()<<"\t\tEndurance: "<<a.endurance<<"\n";
   cout<<"Action name: "<<a.GetActionName()<<"\n";
   cout<<"Position: "<<a.position.x<<" "<<a.position.y<<"\t\tDestination: "<<a.destination.x<<" "<<a.destination.y<<"\n";
   cout<<"\n";
  }

#ifdef _WIN32
  system("cls");
#elif __linux__
  system("clear");
#endif
 }

 return 0;
}
