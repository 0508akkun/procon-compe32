# pragma once

struct Agent
{
    Agent() = default;
    Agent(int _agentID, int _y, int _x);

    int agentID;
    int y, x;
};
