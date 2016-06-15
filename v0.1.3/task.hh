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
   atina::mob_type & mob;

  private:
   virtual status OnEmpty() = 0;
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

   status Update(task_key) const;

   char const * GetActionName();
  private:
   Task () = default;

   std::unique_ptr<task_base> ptr;
 };
};
