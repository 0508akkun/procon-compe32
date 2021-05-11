# pragma once
# include <vector>
# include "Agent.h"

struct Team
{
    Team() = default;
    Team(int _teamID, const std::vector<Agent>& _agents, int _tilePoint, int _areaPoint);
    int teamID;
    std::vector<Agent> agents;
    int tilePoint;
    int areaPoint;
};
