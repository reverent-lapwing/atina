#pragma once

#include <stack>

#include "task.hh"

namespace Mob
{
 class Mob;
}

namespace AI
{
 using task_type = Task::Task;
 using Behaviour = std::function<task_type(mob_type &)>;

 class AI
 {
  public:
   AI( mob_type &, Behaviour );

   AI( AI const & ) = delete;
   AI( AI && ) = delete;

   void Push( task_type );
   void Set();

   std::string GetTaskName();
   std::string GetActionName();
  private:
   mob_type & mob;
   task_type * current {nullptr};
   std::stack<task_type> tasks;
   Behaviour behaviour;
 };
}
