#pragma once

#include <deque>
#include <functional>
#include <initializer_list>

#include "action.hh"
#include "directed_graph.hh"

namespace AI
{
 class AI;
}

namespace Task
{
 class _task_base;
 class Task;

 using action_type = Action::Action;
 struct decision_type {std::function<bool(AI::mob_type const &)> condition; std::deque<action_type> actions;};
 using decision_tree_type = data_struct::directed_graph<decision_type>;

 enum class status {finished, not_finished};

 using _task_base_key = PassKey<Task>;
 using _task_key = PassKey<AI::AI>;

 class _task_base
 {
  public:
   virtual std::unique_ptr<_task_base> Copy() = 0;
   status Top(_task_base_key);
   std::string GetActionName(_task_base_key);

   virtual ~_task_base() = default;

  protected:
   _task_base(AI::mob_type & mob);

   std::deque<action_type> data;
   action_type * current {nullptr};
   AI::mob_type & mob;

  private:
   virtual decltype(current) GetNext() = 0;
 };

 class Task
 {
  public:
   std::string name {"no_name_specified"};

   Task ( Task const & );
   Task ( Task && );

   Task ( std::string name, AI::mob_type &, std::initializer_list<action_type> );
   Task ( std::string name, AI::mob_type &, data_struct::directed_graph<decision_type> const & );

   status Top(_task_key) const;

   std::string GetActionName();
  private:
   Task () = default;

   std::unique_ptr<_task_base> ptr;
 };

// those two tasks must be implemented to allow interuption of tasks
 Task Continue( AI::mob_type & );
 Task Interrupt( AI::mob_type & );
};
