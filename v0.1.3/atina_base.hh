#include <deque>
#include <queue>
#include <stack>

#include "mob_declaration.hh"
#include "utils/passkey.hh"
#include "utils/directed_graph.hh"

namespace atina
{
 enum class status {finished, not_finished};


// AtinA classes
 class action_base;
 class Action;

 class task_base;
 class Task;

 class AI;


// defines type of objects stored in Task and AI
 using action_type = Action;
 using task_type = Task;


// defines containers used in Task and AI
 using task_container_type = std::queue<action_type>;
 using ai_container_type = std::deque<task_type>;


// passkeys
 using task_base_key = utils::PassKey<Task>;
 using task_key = utils::PassKey<AI>;

 using action_base_key = utils::PassKey<Action>;
 using action_key = utils::PassKey<task_base>;


// data structures
 struct decision_type {std::function<bool(atina::mob_type const &)> condition; std::deque<action_type> actions;};
 using  decision_tree_type = utils::directed_graph<decision_type>;

 using Behaviour = std::function<task_type(mob_type &)>;
}
