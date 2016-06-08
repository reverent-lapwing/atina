#include "AI.hh"

using namespace std;
using namespace atina;

void AI::Push( task_type task )
{
 if( current )
 {
  tasks.push(atina::Continue(mob));
  tasks.push(task);
  tasks.push(atina::Interrupt(mob));
 }
 else
 {
  tasks.push(task);
 }

 current = &tasks.top();
}

void AI::Set()
{
 if( not current )
 {
  if( tasks.empty() )
  {
   tasks.push(behaviour(mob));
  }

  current = &tasks.top();
 }

 if( current -> Top({}) == status::finished )
 {
  current = nullptr;
  tasks.pop();
 }
}

char const * AI::GetTaskName()
{
 if( current )
 {
  return current -> name;
 }
 else
 {
  return "notasks_queued";
 }
}
char const * AI::GetActionName()
{
 if( current )
 {
  return current -> GetActionName();
 }
 else
 {
  return "notasks_queued";
 }
}


AI::AI( mob_type & mob, Behaviour behaviour )
: mob(mob)
, behaviour(behaviour)
, current (nullptr)
{}
