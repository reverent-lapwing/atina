#include "AI.hh"

using namespace std;
using namespace atina;

void AI::Interrupt( task_type task, task_type interruption, task_type continuation )
{
 if( data.empty() )
 {
  data.push_front(task);
 }
 else
 {
  data.push_front(continuation);
  data.push_front(task);
  data.push_front(interruption);
 }
}
void AI::Interrupt( task_type task )
{
 data.push_front(task);
}
void AI::Queue( task_type task )
{
 data.push_back( task );
}

void AI::Update()
{
 if( data.empty() )
 {
  data.push_front(behaviour(mob));
 }

 if( data.front().Update({}) == status::finished )
 {
  data.pop_front();
 }
}

char const * AI::GetTaskName()
{
 if( data.empty() )
 {
  return "no_tasks_queued";
 }
 else
 {
  return data.front().name;
 }
}
char const * AI::GetActionName()
{
 if( data.empty() )
 {
  return "no_tasks_queued";
 }
 else
 {
  return data.front().GetActionName();
 }
}


AI::AI( mob_type & mob, Behaviour behaviour )
: mob(mob)
, behaviour(behaviour)
{}
