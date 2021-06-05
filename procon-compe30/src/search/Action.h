# pragma once
# include <string>
# include <vector>

struct Action
{
    Action() = default;
    Action(int _agentID, std::string _type, int _dy, int _dx, int _turn = 0, int _apply = 1);
    int agentID;
    std::string type;
    int dy, dx;
    int turn;
    int apply;
};

std::string ToJson(const std::vector<Action>& actions);

