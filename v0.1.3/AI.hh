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

   void Interrupt( task_type );
   void Interrupt( task_type, task_type, task_type );
   void Queue( task_type );
   void Update();

   char const * GetTaskName();
   char const * GetActionName();
  private:
   mob_type & mob;
   ai_container_type data;
   Behaviour behaviour;
 };
}
