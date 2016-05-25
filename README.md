# AtinA v0.1.0

## ABOUT

__AtinA__ (Athene's incredible AI) is an AI engine for my main project __Reborn__. It's fully self-contained, meaning that it will work for any mob implementation. It's only requirement is that mob class (under name specified by user in an alias) exists.

# TABLE OF CONTENT
- [About](#about)
- [Description](#description)
- [To Do](#to-do)
- [Quick overview](#quick-overview)
  - [Mob declaration](#mob-declaration)
  - [Action](#action)
    - [Class definition](#class-definition)
    - [Construction](#construction-1)
  - [Task](#task)
    - [Action list](#action-list)
    - [Static construction](#static-construction)
    - [Dynamic construction](#dynamic-construction)
  - [AI](#ai)
    - [Construction](#construction-2)
    - [Public methods](#public-methods)
- Tips (to do)
 
## DESCRIPTION

__AtinA__ is composed of three layers: `Action`, `Task` and `AI`
- `Action` is a fundamental AI unit, it represent single action, like standing up or walking to a certain destination. When `update` method is called, `Action` access and alters `mob`'s members related to managing AI, for example `destination`. In my code I use setter functions with passkey, but since I don't claim it's the most effective method of encapsulation, I don't enforce it and how `Action` access `mob` I leave to user's discretion.
- `Task` represents a single task, like working or going for a walk. `Task` can be _static_ (evaluated at construction time) or _dynamic_ (evaluated during execution), both of them are wrapped in general `Task` class. `Task` is implemented as a sequence of `Action`s. `condition ? action1 : action2` syntax can be used to customize `Task` at evaluation time.
- `AI` controls what task to perform using `Behaviour` function. When stack with `Task`s gets empty, it calls `Behaviour` function which returns new `Task` according to rules set by user.

## TO DO

- [x] ` ` ship this goddamn thing already
- [ ] ` ` make a complete README.md
- [ ] ` ` add _make_ file
- [ ] ` ` make name in `Task` constructor optional
- [ ] ` ` remove unnecessary dependencies (replace `std::string` with `const char *` etc)
- [ ] ` ` adjust container choice for `AI` and `Task`
- [ ] ` ` remove unused legacy code
- [ ] ` ` more descriptive method names
- [ ] ` ` use less intrusive namespacing and more conventional style
- [ ] ` ` add native `Task` generator or enable condition evaluation for copy constructor
- [ ] ` ` add native tool for pushing `Task` under condition without repushing every call
- [ ] ` ` redesign `AI` and `Task` so it doesn't use pointer member `current` and guarantee no segfault
- [ ] ` ` make decision tree return intigers instead of booleans
- [ ] ` ` develop directed graph further for later use
- [ ] ` ` add support for `__action_name__{__starting_variables__}` instead of `Action::New<__action_name__>(__starting_variables__)` syntax
- [ ] ` ` finish development of scripting language for all elements of __AtinA__
- [ ] ` ` write conversion tool from .tsk, .act and .ai to .hh/.cc
- [ ] ` ` write conversion tool from .tsk, .act and .ai to .hh/.so and .hh/.dll
- [ ] find out why this turns bold automaticaly `and after this` turns normal

## QUICK OVERVIEW

#### MOB DECLARATION

`AI::mob_type` alias is defined in mob_declaration.hh file. It needs to relate to your mob class.

#### ACTION

Sets `mob` behaviour every time `ai.Set()` is called.

###### Class definition:

```c++
class __action_name__ : public Action::_action_base
{
 public:
  std::string name {"__action_name__"} // optional
 
  static unique_ptr<Action::_action_base> New( __starting_variables__  )
  {
   return make_unique<__action_name__>({__starting_variables__});
  }
  unique_ptr<Action::_action_base> Copy()
  {
   return New(__starting_variables__);
  }

  Action::status _update( AI::mob_type & mob )
  {
   __set_mob_state__();

   return __end_condition__ ? Action::status::finished : Action::status::not_finished;
  }

 private:
  action_name( __starting_variables__ )
  : __starting_variables__(__starting_variables__)
  , __other_variables__(__some_values__)
  {}

  __starting_variables__; // immutable, not declared const because actions need to be copy-assignable
  __other_variables__;
}
```

###### Construction

```c++
Action::New<__action_name__>
(
__starting_variables__
)
```

#### TASK

Stores and controls execution of predifined set of `Action`s.

Tip: to evaluate `c ? t : f` statements every time `Task` is pushed into `AI`, create a wrapper function for the constructor. Copy constructor won't re-evaluate those statements since `Task` stores only `Action`s. Such function could look like this:

```c++
Task::Task __task_name__(){ return {__constructor_arguments__}; }
```

###### Action list

Initializer lists with `Action`s are used in both types of `Task`'s constructors. Example:

```c++
{
// variant 1
 __action_static__,
// variant 2
 __condition_1__ ? __action_if_condition__ : Action::None(), // evaluated at construction time
// variant 3
 __condition_2__ ? __action_if_true__ : __action_if_false__ // like the above,
 ...
}
 ```

###### Static construction

```c++
Task::Task __task_name__
{
 "__task_name__",
 __mob_name__,
 __action_list__
};
```

###### Dynamic construction


```c++
Task::decision_tree_type __tree_name__
 {
 // variant 1
  {{__node_if_true__,__node_if_false__},
   {[](AI::mob_type const & mob){ return __condition__ ? true : false; }, // condition evaluated when control reach this node
   {__action_list_1__}}},
 // variant 2
  {{__next__node__},
   {[](AI::mobe_type const & mob){ return true; },
   {__action_list_2__}}},
  ...
 };

Task::Task __task_name__
{
 "__task_name__",
 __mob_name__
 __tree_name__
};
```

Moving control to node `-1` exits the tree and finish the `Task`. `Action` list can be left empty.

#### AI

Stores and controls execution of proceduraly generated set of `Task`s.

###### Construction

```c++
Task::Task __behaviour_function__( AI::mob_type mob )
{
 if( __condition__ )
 {
  return __task_if_condition__;
 }
 
 ...
 
 return __task_if_no_other__;
}

AI::AI __ai_name__
{
 __mob_name__,
 __behaviour_function__
}
```

###### Public methods

- `void Push(Task::Task)` - interupts current `Task` by pushing following `Task`s onto a stack in a following order: `Task::Continue`, task passed as a function argument and `Task::Interrupt`. `Task::Interrupt` and `Task::Continue` are ordinary `Task`s defined by user, wrapped in a function an empty by default.
	 - `Task::Interrupt` contains `Action`s that should be performed before pushed `Task` (place tools from hand into an inventory).
	 - `Task::Continue` contains `Action`s that should be performed before previous `Task` can continue (take from inventory reqired tool).
- `void Set()` - calls `Update` method of current `Action` and manages any changes if it returned __finished__ flag. If there is no `Action` to call, push new Task chosen by algorythym in `Behaviour` function.