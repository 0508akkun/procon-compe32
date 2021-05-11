# include "Command.h"

Command::Command()
       : to({0,0})
       , a(Act::Stay)
{
}

Command::Command(Act _a, Point _to)
       : a(_a)
       , to(_to)
{
}

Commands::Commands(int sz)
{
    cmds.reserve(sz);
    cmds.resize(sz);
}