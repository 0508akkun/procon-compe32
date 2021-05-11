# include "Greedy.h"
# include "ScoreCalculate.h"

using namespace std;

template<class T>bool chmax(T &a, const T &b) { if (a<b) { a=b; return 1; } return 0; }

vector<Action> greedy(const Field& _field,  const Match& match)
{
    Field field = _field;
    vector<Action> actions;

    int team_index = field.getIndex(match).first;
    Team team = field.teams[team_index];
    const int h = field.height;
    const int w = field.width;
    const int allyID = team.teamID;
    const int enemyID = field.teams[!team_index].teamID;

    for (const auto& agent : team.agents)
    {
        int max_score = -114514;
        Action interestingAction = Action(agent.agentID, "stay", 0, 0);
        Point interestiongPos = { 0, 0 };

        Action unhappyAction = Action(agent.agentID, "stay", 0, 0);
        Point unhappyPos = { 0, 0 };
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                Point p = Point(agent.y, agent.x) + Point(dy, dx);

                if (!p.isOver(h, w))
                {
                    if (field.tiled[p.y][p.x] == enemyID)
                    {
                        field.tiled[p.y][p.x] = 0;
                        int score = ScoreCalculate(field.points, field.normalizedTiled(team_index));
                        if (chmax(max_score, score))
                        {
                            interestiongPos = p;
                            interestingAction = Action(agent.agentID, "remove", dy, dx);
                        }
                        field.tiled[p.y][p.x] = enemyID;
                    }
                    if (field.tiled[p.y][p.x] == allyID)
                    {
                        std::random_device rnd;
                        std::mt19937 mt(rnd());
                        std::uniform_int_distribution<> randdr(-1, 1);
                        array<string, 2> type_str = { "move", "remove" };
                        int _dy = randdr(mt);
                        int _dx = randdr(mt);
                        unhappyAction = Action(agent.agentID, type_str[abs(randdr(mt))], _dy, _dx);
                        unhappyPos = p - Point(dy, dx) + Point(_dy, _dx);

                    }
                    if (field.tiled[p.y][p.x] == 0)
                    {
                        field.tiled[p.y][p.x] = allyID;
                        int score = ScoreCalculate(field.points, field.normalizedTiled(team_index));
                        if (chmax(max_score, score))
                        {
                            interestiongPos = p;
                            interestingAction = Action(agent.agentID, "move", dy, dx);
                        }
                        field.tiled[p.y][p.x] = 0;
                    }
                }
            }
        }
        if (interestingAction.type != "stay")
        {
            actions.push_back(interestingAction);
            if (interestingAction.type == "move")
            {
                field.tiled[interestiongPos.y][interestiongPos.x] = allyID;
            }
            else // "remove"
            {
                field.tiled[interestiongPos.y][interestiongPos.x] = 0;
            }
        }
        else //stay
        {
            actions.push_back(unhappyAction);
        }
    }

    return actions;
}

