#include "task.hh"

using namespace std;
using namespace atina;


status task_base::Top(task_base_key)
{
 if( data.empty() )
 {
  current = GetNext();
  if( current == nullptr )
  {
   return status::finished;
  }
 }
 else
 {
  current = &data.front();
 }

 current = (current -> Update({},mob) == status::finished) ? GetNext() : current;

 return current == nullptr ? status::finished : status::not_finished;
}

char const * task_base::GetActionName(task_base_key)
{
 return current ? current -> GetName() : "task_empty";
}

task_base::task_base(atina::mob_type & mob)
: mob(mob)
{}

class StaticTask : public task_base
{
 public:
  StaticTask( atina::mob_type & mob, initializer_list<action_type> list )
  : task_base(mob)
  {
   for( action_type a : list )
   {
    if( not a.Empty() )
    {
     data.push(a);
    }
   }

   current = data.empty() ? nullptr : &data.front();
  };

  unique_ptr<task_base> Copy()
  {
   auto task = make_unique<StaticTask>(mob, initializer_list<action_type>{});
   task->data = data;

   return move(task);
  }

 private:
  auto GetNext() -> decltype(current)
  {
   if( not data.empty() )
   {
    data.pop();
   }

   return data.empty() ? nullptr : & data.front();
  }
};

class DynamicTask : public task_base
{
 public:
  DynamicTask( atina::mob_type & mob, decision_tree_type const & decision_tree )
  : task_base(mob)
  , decision_tree( decision_tree )
  , index( 0 )
  {
   for( action_type const a : decision_tree.get_node(0).get_data().actions )
   {
    Add(a);
   }

   current = data.empty() ? nullptr : &data.front();
  }

  unique_ptr<task_base> Copy()
  {
   auto task = make_unique<DynamicTask>(mob,decision_tree);
   task -> data = data;
   task -> index = index;

   return move(task);
  }

 private:
 // this insane gizmo is absolutely fine since all those functions are implicitly declared as inline
  void Add( action_type const & a )
  {
   data.push( a );
  }
  void Add( deque<action_type> const & actions )
  {
   for( action_type a : actions )
   {
    Add(a);
   }
  }
  void Add( int i )
  {
   Add( decision_tree.get_node(i).get_data().actions );
  }

  auto GetNext() -> decltype(current)
  {
   if( not data.empty() )
   {
    data.pop();
   }

   while( data.empty() )
   {
    auto& node = decision_tree.get_node(index);

    index = node.get_data().condition(mob) ? node.get_child(0) : node.get_child(1);
    if( index == -1 )
    {
     return nullptr;
    }
    Add( index );
   }

   return &data.front();
  }

  decision_tree_type const & decision_tree;
  int index {0};
};


Task::Task ( Task const & original )
: ptr(original.ptr->Copy())
, name(original.name)
{}

Task::Task ( Task && original )
: ptr(move(original.ptr))
, name(original.name)
{}

Task::Task ( char const * name, atina::mob_type & mob, initializer_list<action_type> list )
: ptr(make_unique<StaticTask>(mob, list))
, name(name)
{}

Task::Task ( char const * name, atina::mob_type & mob, decision_tree_type const & decision_tree_type )
: ptr(make_unique<DynamicTask>(mob, decision_tree_type))
, name(name)
{}

Task::Task ( atina::mob_type & mob, initializer_list<action_type> list )
: ptr(make_unique<StaticTask>(mob, list))
, name(name)
{}

Task::Task ( atina::mob_type & mob, decision_tree_type const & decision_tree_type )
: ptr(make_unique<DynamicTask>(mob, decision_tree_type))
, name(name)
{}


status Task::Top(task_key) const
{
 return ptr ? ptr -> Top({}) : status::finished;
}


char const * Task::GetActionName()
{
 return ptr -> GetActionName({});
}


Task atina::Continue( atina::mob_type & mob )
{
 return {"continue", mob, {}};
}
Task atina::Interrupt( atina::mob_type & mob )
{
 return {"interupt", mob, {}};
}
