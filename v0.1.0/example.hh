namespace Action
{
 class Wait;
 class GoTo;
 class Interupt;
}

namespace Task
{
 Task Sleep( Mob::Mob & mob );
 Task Eat(Mob::Mob & mob);
 Task Rest(Mob::Mob & mob);
 Task Roam(Mob::Mob & mob);
 Task Cry(Mob::Mob & mob);
}

namespace AI
{
 Task::Task NPC(Mob::Mob & mob);
 Task::Task Pet(Mob::Mob & mob);
}


