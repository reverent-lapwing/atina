#pragma once

#include <memory>
#include <string>
#include <iostream>

#include "mob_declaration.hh"
#include "passkey.hh"

namespace Task
{
 class _task_base;
 class Task;
}

namespace Action
{
 enum class status {finished, not_finished};

 class _action_base;
 class Action;

 using _action_base_key = PassKey<Action>;
 using _action_key = PassKey<Task::_task_base>;

 class _action_base
 {
  public:
   std::string name = {"no_name_specified"};

   static std::unique_ptr<_action_base> New();
   virtual std::unique_ptr<_action_base> Copy(); // return copy of an object in a state it was created

   status Update(_action_base_key, AI::mob_type &);

   virtual ~_action_base() = default;
  protected:
   _action_base() = default;
   _action_base( std::string );

  private:
   virtual status _update(AI::mob_type &) = 0;
 };

 class Action
 {
  template< class action_class, class... Args > friend Action New(Args&&... args);
  friend Action None();

  public:
   std::string GetName();

   Action( Action const & original );
   Action( Action && original );

   Action & operator= ( Action const & ); // required for std::deque

   status Update(_action_key, AI::mob_type &);

   bool Empty();

  private:
   Action() = default;
   Action( std::unique_ptr<_action_base> &&);

   std::unique_ptr<_action_base> ptr {nullptr};
 };

 template < class action_class, class... Args > Action New(Args&&... args)
 {
  return {std::move(action_class::New(args...))};
 }
 Action None();
};
