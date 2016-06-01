#pragma once

#include <memory>

#include "atina_base.hh"
#include "mob_declaration.hh"
#include "passkey.hh"

namespace atina
{
 class _action_base
 {
  public:
   char const * name = {"no_name_specified"};

   static std::unique_ptr<_action_base> New();
   virtual std::unique_ptr<_action_base> Copy();

   status Update(_action_base_key, atina::mob_type &);

   virtual ~_action_base() = default;
  protected:
   _action_base() = default;
   _action_base( char const * );

  private:
   virtual status _update(atina::mob_type &) = 0;
 };

 class Action
 {
  public:
   template< class action_class, class... Args >
    static Action New(Args&&... args)
    {
     return {std::move(action_class::New(std::forward<Args...>(args...)))};
    };
   static Action None();

   char const * GetName();

   Action( Action && );
   Action( Action const & );

   Action & operator= ( Action const & ); // required for std::deque

   status Update(_action_key, atina::mob_type &);

   bool Empty();

  private:
   Action() = default;
   Action( std::unique_ptr<_action_base> &&);

   std::unique_ptr<_action_base> ptr {nullptr};
 };
};
