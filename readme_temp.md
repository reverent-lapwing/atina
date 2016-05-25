

## HOW TO USE (IN-DEPTH)

First thing you need to do is to tell the engine the name of your mob class in __mob_declaration.hh__. Now `AI::mob_type` is equivalent to your mob class name. Then you can start creating your own __AtinA__ objects. Listed below are things you need to get you started:
- `Behaviour` function (takes `AI::mob_type`, returns object of class `Task::Task`)
- factory functions for `StaticTask`s you used in `Behaviour` function
- directed graphs for `DynamicTask`s you used in `Behaviour` function
- action classes deriving from `Action::_action_base` used in `Task`s
- object of class `AI::AI`
This ordering is not accidental - this is the order in which I write my AI. The nice thing about writing it this way is that it mimics the proces of designing something in head or on paprer - the first thing you think about is what you want to do, and you thing how to do it later.

Let's take an example I included in this repository and look at it piece by piece.

```
Task::Task NPC(Mob::Mob & mob)
{
 if( mob.hunger < (mob.Is(Mob::trait::lazy) ? 0.2 : 0.5))
 {
  return Task::Eat(mob);
 }

 if( Time::dayTime > 16)
 {
  return Task::Sleep(mob);
 }

 if( mob.endurance < (mob.Is(Mob::trait::lazy) ? 0.5 : 0.2))
 {
  return mob.Is(Mob::trait::lazy) ? Task::Sleep(mob) : Task::Rest(mob);
 }

 return Task::Roam(mob);
}
```
