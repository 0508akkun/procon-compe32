# pragma once
# include <string>

struct Match
{
    Match() = default;
    Match(int _id, int _intervalMillis, std::string _matchTo, int _teamID, int _turnMillis, int _turns);

    int id;
    int intervalMillis;
    std::string matchTo;
    int teamID;
    int turnMillis;
    int turns;
};
