#include <iostream>
#include <map>

#include "mob_mockup.hh"

using namespace std;


map<string, Position> town
{
 {"Fred's House", {.2,.3}},
 {"Bob's House", {.4,.1}},
 {"Farm", {1.,-3.2}}
};



namespace Action
{
 class Wait : public _action_base
 {
  public:
   static unique_ptr<_action_base> New( double toWait )
   {
    return make_unique<Wait>(Wait{toWait});
   }
   unique_ptr<_action_base> Copy()
   {
    return New(toWait);
   }

   status _update( Mob::Mob & mob )
   {
    elapsed += Time::fullTick;
    mob.endurance += 2*Time::fullTick;

    return elapsed < toWait ? status::not_finished : status::finished;
   }

  private:
   Wait( double toWait )
   : _action_base("wait")
   , toWait(toWait)
   , elapsed(0)
   {}

   double elapsed;
   double toWait;
 };

 class GoTo : public _action_base
 {
  public:
   static unique_ptr<_action_base> New( Position destination )
   {
    return make_unique<GoTo>(GoTo{destination});
   }
   unique_ptr<_action_base> Copy()
   {
    return New(destination);
   }

   status _update( AI::mob_type & mob )
   {
    mob.destination = destination;

    return mob.position != destination ? status::not_finished : status::finished;
   }

  private:
   GoTo( Position destination )
   : _action_base("go_to")
   , destination(destination)
   {}

   Position destination;
 };

 class Interupt : public _action_base
 {
  public:
   static unique_ptr<_action_base> New( function<Task::Task(Mob::Mob&)> task )
   {
    return make_unique<Interupt>(Interupt{task});
   }
   unique_ptr<_action_base> Copy()
   {
    return New(task);
   }

   status _update( Mob::Mob & mob )
   {
    mob.Interupt(task(mob));

    return status::finished;
   }

  private:
   Interupt( function<Task::Task(Mob::Mob&)> task )
   : _action_base("interupt")
   , task(task)
   {}

   function<Task::Task(Mob::Mob&)> task;
 };
}

namespace Task
{
 Task Sleep( Mob::Mob & mob );
 Task Eat(Mob::Mob & mob);
 Task Rest(Mob::Mob & mob);
 Task Roam(Mob::Mob & mob);
 Task Cry(Mob::Mob & mob);

 decision_tree_type roamDecisionTree
 {
  {{1,2},{[](Mob::Mob const & mob){ return mob.position == town.at("Farm") ? true : false; },{}}},
  {{3},{[](Mob::Mob const & mob){ return true; },{Action::New<Action::Interupt>(Rest)}}},
  {{1,3},{[](Mob::Mob const & mob){ return (mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.7 : 0.5)) ? true : false; },{Action::New<Action::GoTo>(town.at("Farm"))}}},
  {{4,5},{[](Mob::Mob const & mob){ return mob.name == "bob" ? true : false; },{}}},
  {{-1},{[](Mob::Mob const & mob){ return true; },{Action::New<Action::GoTo>(town.at("Bob's House")),Action::New<Action::Interupt>(Rest)}}},
  {{-1, 3},{[](Mob::Mob const & mob){ return (mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.7 : 0.5)) ? true : false; },{Action::New<Action::GoTo>(town.at("Fred's House")),Action::New<Action::GoTo>(town.at("Bob's House")),Action::New<Action::GoTo>(town.at("Farm"))}}}
 };

 Task Sleep( Mob::Mob & mob ){return {"sleep", mob,
 { mob.name == "dog" ? Action::None() : Action::New<Action::GoTo>( mob.name == "fred" ? town.at("Fred's House") : town.at("Bob's House") )
 , Action::New<Action::Wait>(mob.Is(Mob::trait::lazy) ? 10 : 8)}};};
 Task Eat(Mob::Mob & mob){return {"eat", mob, {}};};
 Task Rest(Mob::Mob & mob){return {"rest", mob, {Action::New<Action::Wait>(mob.Is(Mob::trait::lazy) ? 2 : 1)}};};
 Task Roam(Mob::Mob & mob){return {"roam", mob, roamDecisionTree}; cout << "roam_push"; };
 Task Cry(Mob::Mob & mob){return {"cry", mob, {}};};
}

namespace AI
{
 Task::Task NPC(Mob::Mob & mob)
 {
  if( mob.hunger < (mob.Is(Mob::trait::lazy) ? 0.2 : 0.5))
  {
   return Task::Eat(mob);
  }

  if( Time::dayTime > 16)
  {
   return Task::Sleep(mob);
  }

  if( mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.5 : 0.2))
  {
   return mob.Is(Mob::trait::lazy) ? Task::Sleep(mob) : Task::Rest(mob);
  }

  return Task::Roam(mob);
 };

 Task::Task Pet(Mob::Mob & mob)
 {
  if( mob.hunger < 0.5)
  {
   return (mob.hunger < 0.2 ? Task::Eat(mob) : (mob.Is(Mob::trait::lazy) ? Task::Cry(mob) : Task::Eat(mob)));
  }

  if( mob.endurance == 1 )
  {
   return Task::Roam(mob);
  }

  return Task::Rest(mob);
 };
}


Mob::Mob Mob::NewFred()
{
 Mob fred(AI::NPC);

 fred.name = "fred";

 return fred;
}

Mob::Mob Mob::NewBob()
{
 Mob bob{AI::NPC};

 bob.name = "bob";

 return bob;
}

Mob::Mob Mob::NewDog()
{
 Mob dog{AI::Pet};

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
