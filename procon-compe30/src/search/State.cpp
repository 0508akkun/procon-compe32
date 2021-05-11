# include "State.h"

State::State()
{
}

State::State(FFS _field)
     : field(_field)
{
    calcScore();
}
 

void State::calcScore()
{
    score = LeadScoreCalculate(field, 2, 1, 2, 1);
}

State transition(const State& state, const int id, const Point dir, const std::vector<std::vector<Point>>& conflictAllyPos, const int d)
{
    State next = state;
    const Point here = next.field.Allies[id].pos;
    const int width = next.field.width;
    const int height = next.field.height;
    const Point targetPos = here+dir;
    // 停留措置
    if(dir.isZero())
    {
        Command tmpc;
        next.via.emplace_back(std::make_pair(tmpc,std::make_pair(here,here)));
        return next;
    }
    // 場外 // 停留措置
    if(targetPos.isOver(height, width))
    {
        Command tmpc;
        next.via.emplace_back(std::make_pair(tmpc,std::make_pair(here,here)));
        return next; 
    }
    // 他人ゾーン // 停留措置
    for(auto n:conflictAllyPos[d])
    {
        if(targetPos == n)
        {
            Command tmpc;
            next.via.emplace_back(std::make_pair(tmpc,std::make_pair(here,here)));
            return next;
        }
    }

    ATP& targetType = next.field.Board[targetPos.y][targetPos.x].type;
    if(targetType == ATP::enemy)
    {
        targetType = ATP::newtral;
        Command tmpc(Act::Remove, dir);
        next.via.emplace_back(std::make_pair(tmpc,std::make_pair(here,targetPos)));
    }
    else
    {
        targetType = ATP::ally;
        Command tmpc(Act::Move, dir);
        next.via.emplace_back(std::make_pair(tmpc,std::make_pair(here,targetPos)));
        next.field.Allies[id] = targetPos;
    }
    next.calcScore();
    return next;
}
