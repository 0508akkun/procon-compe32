# include "Human.h"
# include "Team.h"
# include "Utility/Box.h"
using namespace std;

vector<Action> human(const Field& field, const Match& match)
{
    vector<Action> actions;

    int team_index = field.getIndex(match).first;
    Team team = field.teams[team_index];

    cout << box::input << "INPUT: Action" << endl;
    cout << "    " << "[act]: \"[m]ove\", \"[r]emove\", \"[s]tay\"" << endl;
    cout << "    " << "[dir]: dy dx" << endl;
    // cout << "    " << "b 0 0で一つ前の入力に戻れます" << endl;
    // cout << "    " << "入力形式: act y x" << endl;
    cout << endl;
    for (int i = 0; i < (int)team.agents.size(); ++i)
    {
        cout << box::check << "AgentID: " << team.agents[i].agentID << endl;
        cout << box::check << "Pos: [" << team.agents[i].y << "][" << team.agents[i].x << "]" << endl;
        cout << box::input << "[act & dir]>>> ";
        char act;
        int dy, dx;
        cin >> act;
        cin >> dy >> dx;

        if (act == 'b' && dy == 0 && dx == 0)
        {
            if (!actions.empty())
            {
                actions.pop_back();
            }
            i -= 2;
            continue;
        }

        switch (act)
        {
        case 'm':
            actions.push_back(Action(team.agents[i].agentID, "move", dy, dx));
            break;
        case 'r':
            actions.push_back(Action(team.agents[i].agentID, "remove", dy, dx));
            break;
        case 's':
            actions.push_back(Action(team.agents[i].agentID, "stay", 0, 0));
            break;
        }
    }

    return actions;
}
