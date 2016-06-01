#include "action.hh"

using namespace std;
using namespace atina;

unique_ptr<_action_base> _action_base::New()
{
 return nullptr;
}

unique_ptr<_action_base> _action_base::Copy()
{
 return nullptr;
}

status _action_base::Update(_action_base_key, atina::mob_type & m)
{
 return _update( m );
}

_action_base::_action_base( char const * name )
: name(name)
{}

Action::Action ( Action && original )
: ptr (move(original.ptr))
{}

Action::Action ( Action const & original )
: ptr (original.ptr -> Copy())
{}

Action & Action::operator= ( Action const & original )
{
 ptr = original.ptr -> Copy();

 return *this;
}

Action::Action ( unique_ptr<_action_base> && new_ptr )
: ptr(move(new_ptr))
{}

status Action::Update(_action_key, atina::mob_type & m)
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
