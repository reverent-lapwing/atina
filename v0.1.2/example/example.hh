#include "mob_mockup.hh"

namespace Actions
{
 class Wait;
 class GoTo;
 class Interupt;
}

namespace Tasks
{
 atina::Task Sleep( Mob::Mob & mob );
 atina::Task Eat(Mob::Mob & mob);
 atina::Task Rest(Mob::Mob & mob);
 atina::Task Roam(Mob::Mob & mob);
 atina::Task Cry(Mob::Mob & mob);
}

namespace AIs
{
 atina::Task NPC(Mob::Mob & mob);
 atina::Task Pet(Mob::Mob & mob);
}


