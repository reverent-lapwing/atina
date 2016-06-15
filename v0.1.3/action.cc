#include "action.hh"

using namespace std;
using namespace atina;

Action action_base::New()
{
 return Action::None();
}

Action action_base::Copy()
{
 return Action::None();
}

status action_base::Update(action_base_key, atina::mob_type & m)
{
 return update( m );
}

action_base::action_base( char const * name )
: name(name)
{}

Action::Action ( Action && original )
: ptr (move(original.ptr))
{}

Action::Action ( Action const & original )
: ptr (original.ptr -> Copy().ptr)
{}

Action & Action::operator= ( Action const & original )
{
 ptr = original.ptr -> Copy().ptr;

 return *this;
}

Action::Action ( unique_ptr<action_base> && new_ptr )
: ptr(move(new_ptr))
{}

status Action::Update(action_key, atina::mob_type & m)
{
 return ptr ? ptr -> Update({},m) : status::finished;
};

bool Action::Empty()
{
 return ptr == nullptr;
}

char const * Action::GetName()
{
 return ptr ? ptr -> name : "none";
}


Action Action::None()
{
 return Action();
}
