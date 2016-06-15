# AtinA

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
    - [Construction](#construction-0)
  - [Task](#task)
    - [Action list](#action-list)
    - [Static construction](#static-construction)
    - [Dynamic construction](#dynamic-construction)
  - [AI](#ai)
    - [Construction](#construction-1)
    - [Public methods](#public-methods)
- Tutorial (to do)

## DESCRIPTION

__AtinA__ is composed of three layers: `Action`, `Task` and `AI`
- `Action` is a fundamental AI unit, it represent single action, like standing up or walking to a certain destination. When `Update` method is called, `Action` access and alters `mob`'s members related to managing AI, for example `destination`. In my code I use setter functions with passkey, but since I don't claim it's the most effective method of encapsulation, I don't enforce it and how `Action` access `mob` I leave to user's discretion.
- `Task` represents a single task, like working or going for a walk. `Task` can be _static_ (evaluated at construction time) or _dynamic_ (evaluated during execution), both of them are wrapped in general `Task` class. `Task` is implemented as a sequence of `Action`s. `condition ? action1 : action2` syntax can be used to customize `Task` at evaluation time.
- `AI` controls what task to perform using `Behaviour` function. When stack with `Task`s gets empty, it calls `Behaviour` function which returns new `Task` according to rules set by user.

## TO DO

- [x] ship this goddamn thing already
- [ ] make a complete README.md
- [ ] add _make_ file
- [x] make name in `Task` constructor optional
- [x] make name in `Action` class optional
- [x] remove unnecessary dependencies (replace `std::string` with `const char *` etc)
- [x] adjust container choice for `AI` and `Task`
- [x] ~~move Interupt and Continue to a separate file~~ no longer required
- [ ] remove unused legacy code
- [x] more descriptive method names
- [ ] replace __mob_declaration.hh__ with implicit template if possible (and practical)
- [x] use less intrusive namespacing and more ~~conventional~~ consistent style
- [ ] add native `Task` generator or enable condition evaluation for copy constructor
- [ ] add native tool for pushing `Task` to `AI` under condition without repushing every call (see Mob::Mob::Update() in mob_mockup.cc)
- [x] redesign `AI` and `Task` so it doesn't use pointer member `current` and guarantee no segfault
- [ ] make decision tree return integers instead of booleans
- [ ] develop (directed) graph further for later use
- [x] add support for `__action_name__{__starting_variables__}` instead of `atina::Action::New<__action_name__>(__starting_variables__)` syntax
- [ ] finish development of scripting language for all elements of __AtinA__
- [ ] write conversion tool from .tsk, .act and .ai to .hh/.cc
- [ ] write conversion tool from .tsk, .act and .ai to .hh/.so and .hh/.dll
- [x] ~~find out why this turns bold automatically `and after this` turns normal~~ apparently it doesn't

## QUICK OVERVIEW

#### SETUP

In order to use __AtinA__ include "atina.hh" in your code and link `atina::mob_type` alias to your mob class in __mob_declaration.hh__ header.

#### ACTION

Sets `mob` behaviour every time `AI`'s `Update()` method is called.

###### Class definition:

```c++
class __action_name__ : public atina::action_base
{
 public:
  char const * name {"__action_name__"} // optional

  static atina::Action New( __starting_variables__ )
  {
   return {make_unique<__action_name__>({__starting_variables__})};
  }
  atina::Action Copy()
  {
   return New(__starting_variables__);
  }

  status update( atina::mob_type & mob )
  {
   __set_mob_state__();

   return __end_condition__ ? status::finished : status::not_finished;
  }

 private:
  __action_name__( __starting_variables__ )
  : __starting_variables__(__starting_variables__)
  , __other_variables__(__some_values__)
  {}

  const __starting_variables__;
  __other_variables__;
}
```

###### Construction

```c++
__action_name__::New
(
__starting_variables__
)
```

#### TASK

Stores and controls execution of predefined set of `Action`s.

Tip: to evaluate `c ? t : f` statements every time `Task` is pushed into `AI`, create a wrapper function for the constructor. Copy constructor won't re-evaluate those statements since `Task` stores only `Action`s. Such function could look like this:

```c++
Task::Task _task_name__(){ return {__constructor_arguments__}; }
```

###### Action list

Initializer lists with `Action`s are used in both types of `Task`'s constructors. Example:

```c++
{
// variant 1
 _action_static__,
// variant 2
 __condition_1__ ? _action_if_condition__ : atina::Action::None(), // evaluated at construction time
// variant 3
 __condition_2__ ? _action_if_true__ : _action_if_false__ // like the above,
 ...
}
```

###### Static construction

```c++
atina::Task __task_name__
{
 "__task_name__", // optional
 __mob_name__,
 _action_list__
};
```

###### Dynamic construction


```c++
atina::decision_tree_type __tree_name__
 {
 // variant 1
  {{__node_if_true__,__node_if_false__},
   {[](atina::mob_type const & mob){ return __condition__ ? true : false; }, // condition evaluated when control reach this node
   {_action_list_1__}}},
 // variant 2
  {{__next__node__},
   {[](atina::mobe_type const & mob){ return true; },
   {_action_list_2__}}},
  ...
 };

atina::Task __task_name__
{
 "__task_name__", // optional
 __mob_name__,
 __tree_name__
};
```

Moving control to node `-1` exits the tree and finish the `Task`. `Action` list can be left empty.

#### AI

Stores and controls execution of procedurally generated set of `Task`s.

###### Construction

```c++
atina::Task __behaviour_function__( atina::mob_type mob )
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

- `void Interrupt(atina::Task task)`
  interrupts current `Task` by pushing `task` in front of the queue
- `void Interrupt(atina::Task task, atina::Task interruption, atina::Task continuation)`
  interrupts current `Task` by pushing `task` in front of the queue after `interruption` and before `continuation`
- `void Queue(atina::Task task)`
  add `task` to the end of the queue
- `void Update()` - calls `Update` method of current `Action` and manages any changes if it returned __finished__ flag. If there is no `Action` to call, push new `Task` chosen by algorithm specified in `Behaviour` function.
