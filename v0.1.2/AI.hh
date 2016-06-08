#pragma once

#include "task.hh"

namespace atina
{
 class AI
 {
  public:
   AI( mob_type &, Behaviour );

   AI( AI const & ) = delete;
   AI( AI && ) = delete;

   void Push( task_type );
   void Set();

   char const * GetTaskName();
   char const * GetActionName();
  private:
   mob_type & mob;
   task_type * current {nullptr};
   ai_container_type tasks;
   Behaviour behaviour;
 };
}
