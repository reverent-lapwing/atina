#pragma once

#include <memory>

#include "atina_base.hh"

namespace atina
{
 class action_base
 {
  public:
   static std::unique_ptr<action_base> New();
   virtual std::unique_ptr<action_base> Copy();

   status Update(action_base_key, atina::mob_type &);

   virtual ~action_base() = default;

   char const * name = {"no_name_specified"};

  protected:
   action_base() = default;
   action_base( char const * );

  private:
   virtual status _update(atina::mob_type &) = 0;
 };

 class Action
 {
  public:
    template< class action_class, class... Args >
   static Action New(Args&&... args);
   static Action None();

   char const * GetName();

   Action( Action && );
   Action( Action const & );

   Action & operator= ( Action const & ); // required for std::deque

   status Update( action_key, atina::mob_type & );

   bool Empty();

  private:
   Action() = default;
   Action( std::unique_ptr<action_base> &&);

   std::unique_ptr<action_base> ptr {nullptr};
 };
};


 template< class action_class, class... Args >
atina::Action atina::Action::New( Args&&... args )
{
 return {std::move(action_class::New(std::forward<Args...>(args...)))};
};
