#include <deque>

#include "mob_declaration.hh"
#include "passkey.hh"
#include "directed_graph.hh"

namespace atina
{
 enum class status {finished, not_finished};


 class _action_base;
 class Action;

 class _task_base;
 class Task;

 class AI;


 using action_type = Action;
 using task_type = Task;


 using _task_base_key = utils::PassKey<Task>;
 using _task_key = utils::PassKey<AI>;

 using _action_base_key = utils::PassKey<Action>;
 using _action_key = utils::PassKey<_task_base>;


 struct decision_type {std::function<bool(atina::mob_type const &)> condition; std::deque<action_type> actions;};
 using  decision_tree_type = utils::directed_graph<decision_type>;

 using Behaviour = std::function<task_type(mob_type &)>;
}
