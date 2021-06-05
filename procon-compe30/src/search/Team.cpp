# include "Team.h"

Team::Team(int _teamID, const std::vector<Agent>& _agents, int _tilePoint, int _areaPoint)
    : teamID(_teamID)
    , agents(_agents)
    , tilePoint(_tilePoint)
    , areaPoint(_areaPoint)
{}