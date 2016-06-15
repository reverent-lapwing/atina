#pragma once

#include <memory>

#include "atina_base.hh"

namespace atina
{
 class action_base
 {
  public:
   static Action New();
   virtual Action Copy();

   status Update(action_base_key, atina::mob_type &);

   virtual ~action_base() = default;

   char const * name = {"no_name_specified"};

  protected:
   action_base() = default;
   action_base( char const * );

  private:
   virtual status update(atina::mob_type &) = 0;
 };

 class Action
 {
  public:
   static Action None();

   char const * GetName();

   Action( std::unique_ptr<action_base> && );

   Action( Action && );
   Action( Action const & );

   Action & operator= ( Action const & ); // required for std::deque

   status Update( action_key, atina::mob_type & );

   bool Empty();

  private:
   Action() = default;

   std::unique_ptr<action_base> ptr {nullptr};
 };
};
