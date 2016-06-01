#include "action.hh"

using namespace std;
using namespace Action;

unique_ptr<_action_base> _action_base::New()
{
 return nullptr;
}

unique_ptr<_action_base> _action_base::Copy()
{
 return nullptr;
}

status _action_base::Update(_action_base_key, AI::mob_type & m)
{
 return _update( m );
}

_action_base::_action_base( std::string name )
: name(name)
{}


Action::Action::Action ( Action const & original )
: ptr (original.ptr->Copy())
{}

Action::Action::Action ( Action && original )
: ptr (move(original.ptr))
{}

Action::Action & Action::Action::operator= ( Action const & original )
{
 ptr = original.ptr -> Copy();

 return *this;
}

Action::Action::Action ( unique_ptr<_action_base> && new_ptr )
: ptr(move(new_ptr))
{}

status Action::Action::Update(_action_key, AI::mob_type & m)
{
 return ptr ? ptr -> Update({},m) : status::finished;
};

bool Action::Action::Empty()
{
 return ptr == nullptr;
}

string Action::Action::GetName()
{
 return ptr ? ptr -> name : "none";
}

Action::Action Action::None()
{
 return Action();
}
