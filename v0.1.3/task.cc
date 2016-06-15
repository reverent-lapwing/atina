#include "task.hh"

using namespace std;
using namespace atina;


status task_base::Top(task_base_key)
{
 if( not data.empty() )
 {
  if( data.front().Update({},mob) == status::finished )
  {
   data.pop();
  }
 }

 return data.empty() ? OnEmpty() : status::not_finished;
}

char const * task_base::GetActionName(task_base_key)
{
 return data.empty() ? "task_empty" : data.front().GetName();
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
  };

  unique_ptr<task_base> Copy()
  {
   auto task = make_unique<StaticTask>(mob, initializer_list<action_type>{});
   task->data = data;

   return move(task);
  }

  status OnEmpty()
  {
   return status::finished;
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

  status OnEmpty()
  {
   while( data.empty() )
   {
    auto& node = decision_tree.get_node(index);

    index = node.get_data().condition(mob) ? node.get_child(0) : node.get_child(1);
    if( index == -1 )
    {
     return status::finished;
    }
    Add( index );
   }

   return status::not_finished;
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
{}

Task::Task ( atina::mob_type & mob, decision_tree_type const & decision_tree_type )
: ptr(make_unique<DynamicTask>(mob, decision_tree_type))
{}


status Task::Update(task_key) const
{
 return ptr ? ptr -> Top({}) : status::finished;
}


char const * Task::GetActionName()
{
 return ptr -> GetActionName({});
}
