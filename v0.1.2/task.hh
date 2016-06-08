#pragma once

#include <deque>
#include <functional>
#include <initializer_list>

#include "action.hh"

namespace atina
{
 class task_base
 {
  public:
   virtual std::unique_ptr<task_base> Copy() = 0;
   status Top(task_base_key);
   char const * GetActionName(task_base_key);

   virtual ~task_base() = default;

  protected:
   task_base(atina::mob_type & mob);

   task_container_type data;
   action_type * current {nullptr};
   atina::mob_type & mob;

  private:
   virtual decltype(current) GetNext() = 0;
 };

 class Task
 {
  public:
   char const * name {"no_name_specified"};

   Task ( Task const & );
   Task ( Task && );

   Task ( char const * name, atina::mob_type &, std::initializer_list<action_type> );
   Task ( char const * name, atina::mob_type &, atina::decision_tree_type const & );

   Task ( atina::mob_type &, std::initializer_list<action_type> );
   Task ( atina::mob_type &, atina::decision_tree_type const & );

   status Top(task_key) const;

   char const * GetActionName();
  private:
   Task () = default;

   std::unique_ptr<task_base> ptr;
 };

// those two tasks must be implemented to allow interuption of tasks
 Task Continue( atina::mob_type & );
 Task Interrupt( atina::mob_type & );
};
