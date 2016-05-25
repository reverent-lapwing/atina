#include "AI.hh"

#include "mob_mockup.hh"

using namespace std;
using namespace AI;

void AI::AI::Push( task_type task )
{
 if( current )
 {
  tasks.push(Task::Continue(mob));
  tasks.push(task);
  tasks.push(Task::Interrupt(mob));
 }
 else
 {
  tasks.push(task);
 }

 current = &tasks.top();
}

void AI::AI::Set()
{
 if( not current )
 {
  if( tasks.empty() )
  {
   tasks.push(behaviour(mob));
  }

  current = &tasks.top();
 }

 if( current -> Top({}) == Task::status::finished )
 {
  current = nullptr;
  tasks.pop();
 }
}

std::string AI::AI::GetTaskName()
{
 if( current )
 {
  return current -> name;
 }
 else
 {
  return "no_tasks_queued";
 }
}
std::string AI::AI::GetActionName()
{
 if( current )
 {
  return current -> GetActionName();
 }
 else
 {
  return "no_tasks_queued";
 }
}


AI::AI::AI( mob_type & mob, Behaviour behaviour )
: mob(mob)
, behaviour(behaviour)
, current (nullptr)
{}
