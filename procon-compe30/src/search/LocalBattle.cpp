# include "Battle.h"
# include "AccessServer.h"
# include "Utility/Box.h"
# include "Field.h"
# include "FieldReader.h"
# include "Match.h"
# include "beamsearch.h"
# include "beamsearch2.h"
# include "beamsearch3.h"
# include "beamsearch4.h"
# include "beamsearch5.h"
# include "beamsearch6.h"
# include "beamsearch7.h"
# include "Greedy.h"
# include "Human.h"
# include "OptionArg.h"
# include "picojson/picojson.h"
# include <bits/stdc++.h>

using namespace std;

bool excludeInvalidActions(const Field&, int, vector<Action>&, vector<Action>& actions_buff);
bool move(Field& field, int team_index, vector<Action>& actions, vector<Action>& actions_buff);
bool remove(Field& field, int team_index, vector<Action>& actions, vector<Action>& actions_buff);
bool exec(Field& field, vector<Action> red_actions, vector<Action> blue_actions);
bool Log(const Field& field, const vector<Action> red_actions, const vector<Action> blue_actions, int turn);
bool Log(string dir_path, const Field& field, const vector<Action> red_actions, const vector<Action> blue_actions, int turn, char red_selelct, char blue_selelect);

void LocalBattle(const Field& field)
{
    cout << endl << box::disp << "DISPLAY: 初期フィールド" << endl;
    DisplayField(field, field.teams[0].teamID, field.teams[1].teamID);

    string dir_path = OptionArg::Get<string>("output");
    if (dir_path == "path/to/output")
    {
        dir_path = "";
    }
    LogField(field, dir_path + "log/field.txt");

    map<char, function<vector<Action>(const Field&, const Match&)>> solver;
    solver['1'] = solve_beam;
    solver['2'] = solve_beam2;
    solver['3'] = solve_beam3;
    solver['4'] = solve_beam4;
    solver['5'] = ngng::solve_beam5;
    solver['6'] = ngng::solve_beam6;
    solver['7'] = pointn::solve_beam7;
    solver['s'] = pointn::solve_beam7_2;
    solver['g'] = greedy;
    // solver['r'] = randomized;
    solver['h'] = human;

    cout << box::input << "対戦アルゴリズムを選択(赤)" << endl;
    cout << "    " << "[1]eamsearch" << endl;
    cout << "    " << "[2]eamsearch" << endl;
    cout << "    " << "[3]eamsearch" << endl;
    cout << "    " << "[4]eamsearch" << endl;
    cout << "    " << "[5]eamsearch" << endl;
    cout << "    " << "[6]eamsearch" << endl;
    cout << "    " << "[7]eamsearch" << endl;
    cout << "    " << "[s]eamsearch" << endl;
    cout << "    " << "[g]reedy algorithm" << endl;
    cout << "    " << Color::CharRed << "[r]randomized algorithm" << Color::Reset << endl;
    cout << "    " << "[h]uman" << endl;
    char red_select;
    cout << box::input << ">>> ";
    cin >> red_select;

    cout << box::input << "対戦アルゴリズムを選択(青)" << endl;
    cout << "    " << "[1]eamsearch" << endl;
    cout << "    " << "[2]eamsearch" << endl;
    cout << "    " << "[3]eamsearch" << endl;
    cout << "    " << "[4]eamsearch" << endl;
    cout << "    " << "[5]eamsearch" << endl;
    cout << "    " << "[6]eamsearch" << endl;
    cout << "    " << "[7]eamsearch" << endl;
    cout << "    " << "[s]eamsearch" << endl;
    cout << "    " << "[g]reedy algorithm" << endl;
    cout << "    " << Color::CharRed << "[r]randomized algorithm" << Color::Reset << endl;
    cout << "    " << "[h]uman" << endl;
    char blue_select;
    cout << box::input << ">>> ";
    cin >> blue_select;

    function<vector<Action>(const Field&, const Match&)> redSolver = solver[red_select];
    function<vector<Action>(const Field&, const Match&)> blueSolver = solver[blue_select];

    int max_turn = OptionArg::Get<int>("turns");
    int turnMillis = OptionArg::Get<int>("turnms");
    Match red_match(0, 0, "null", field.teams[0].teamID, turnMillis, max_turn);
    Match blue_match(0, 0, "null", field.teams[1].teamID, turnMillis, max_turn);
 
    Field nowField = field;
    int team_index = 1;

    vector<Action> red_actions;
    vector<Action> blue_actions;
    for (int turn = 1; turn <= max_turn; ++turn)
    {
        nowField.turn = turn;

        DisplayField(nowField, field.teams[0].teamID, field.teams[1].teamID);

        int red_score = ScoreCalculate(nowField.points, nowField.normalizedTiled(0));
        int red_tileScore = TileCalculate(nowField.points, nowField.normalizedTiled(0));
        int red_areaScore = AreaCalculate(nowField.points, nowField.normalizedTiled(0));

        int blue_score = ScoreCalculate(nowField.points, nowField.normalizedTiled(1));
        int blue_tileScore = TileCalculate(nowField.points, nowField.normalizedTiled(1));
        int blue_areaScore = AreaCalculate(nowField.points, nowField.normalizedTiled(1));

        cout << box::disp << "Turn : " << turn << "/" << max_turn << endl;
        cout << box::disp << Color::BackRed << " " << red_select << " " << Color::Reset << red_score;
                     cout << " vs ";
                     cout << blue_score << Color::BackCyan << " " << blue_select << " " << Color::Reset << endl;
        cout << box::disp << "(" << red_tileScore << " + " << red_areaScore << ")";
                     cout << " vs ";
                     cout << "(" << blue_tileScore << " + " << blue_areaScore << ")" << endl;
        red_actions = redSolver(nowField, red_match);
        blue_actions = blueSolver(nowField, blue_match);

        cout << endl;

        exec(nowField, red_actions, blue_actions);

        Log(dir_path, nowField, red_actions, blue_actions, turn, red_select, blue_select);
    }

    DisplayField(nowField, field.teams[0].teamID, field.teams[1].teamID);

    int red_score_final = ScoreCalculate(nowField.points, nowField.normalizedTiled(0));
    int red_tilescore_final = TileCalculate(nowField.points, nowField.normalizedTiled(0));
    int red_areascore_final = AreaCalculate(nowField.points, nowField.normalizedTiled(0));

    int blue_score_final = ScoreCalculate(nowField.points, nowField.normalizedTiled(1));
    int blue_tilescore_final = TileCalculate(nowField.points, nowField.normalizedTiled(1));
    int blue_areascore_final = AreaCalculate(nowField.points, nowField.normalizedTiled(1));

        cout << box::disp << "Turn : " << "Last" << endl;
        cout << box::disp << Color::BackRed << " " << red_select << " " << Color::Reset << red_score_final;
                     cout << " vs ";
                     cout << blue_score_final << Color::BackCyan << " " << blue_select << " " << Color::Reset << endl;
        cout << box::disp << "(" << red_tilescore_final << " + " << red_areascore_final << ")";
                     cout << " vs ";
                     cout << "(" << blue_tilescore_final << " + " << blue_areascore_final << ")" << endl;

    Log(dir_path, nowField, red_actions, blue_actions, max_turn, red_select, blue_select);
}



bool exec(Field& field, vector<Action> red_actions, vector<Action> blue_actions)
{
    // 無駄な処理だけど計算量そんなにないので確実に処理するために書いとくよ
    // 一旦Actionはすべて有効であるものとする
    for (auto& act : red_actions)
    {
        act.apply = 1;
    }
    for (auto& act : blue_actions)
    {
        act.apply = 1;
    }

    assert(red_actions.size() == field.teams[0].agents.size());
    assert(blue_actions.size() == field.teams[1].agents.size());

    // 「無効」や「競合」そして「有効」な行動を順次格納
    vector<Action> actions_buff;

    // 「無効」な行動をAction配列から排除する
    excludeInvalidActions(field, 0, red_actions, actions_buff);
    excludeInvalidActions(field, 1, blue_actions, actions_buff);

    // 指定先がかぶった場合、その行動は競合となる
    map<int, int> mp;
    for (auto& act : red_actions)
    {
        Team team = field.teams[0];
        for (int i = 0; i < team.agents.size(); ++i)
        {
            if (team.agents[i].agentID == act.agentID)
            {
                int ty = team.agents[i].y + act.dy;
                int tx = team.agents[i].x + act.dx;
                mp[1000*ty + tx]++;
            }
        }
    }
    for (auto& act : blue_actions)
    {
        Team team = field.teams[1];
        for (int i = 0; i < team.agents.size(); ++i)
        {
            if (team.agents[i].agentID == act.agentID)
            {
                int ty = team.agents[i].y + act.dy;
                int tx = team.agents[i].x + act.dx;
                mp[1000*ty + tx]++;
            }
        }
    }

    auto conflict = [&](auto& act){ 
        for (const auto& team : field.teams)
        {
            for (const auto& agent : team.agents)
            {
                if (agent.agentID == act.agentID)
                {
                    int ty = agent.y + act.dy;
                    int tx = agent.x + act.dx;
                    if (mp[1000*ty + tx] >= 2)
                    {
                        cout << box::failure << "CONFLICT" << endl;
                        cout << "agentID: " << agent.agentID << endl;
                        cout << "agentPos: (" << agent.y << ", " << agent.x << ")" << endl;
                        cout << "agentAct: " << act.type  << "(" << act.dy << ", " << act.dx << ")" << endl;
                        cout << endl;

                        act.apply = 0;
                        actions_buff.push_back(act);

                        return true;
                    }
                }
            }
        }
        return false;
    };

    auto it = find_if(red_actions.begin(), red_actions.end(), conflict);
    while(it != red_actions.end())
    {
        int index = std::distance(red_actions.begin(), it);
        red_actions[index].apply = 0;
        it = find_if(++it, red_actions.end(), conflict);
    }

    it = find_if(blue_actions.begin(), blue_actions.end(), conflict);
    while(it != blue_actions.end())
    {
        int index = std::distance(blue_actions.begin(), it);
        blue_actions[index].apply = 0;
        it = find_if(++it, blue_actions.end(), conflict);
    }

    // 競合した行動を排除
    auto result = remove_if(red_actions.begin(), red_actions.end(), [](auto& act){
        return act.apply == 0;
    });
    red_actions.erase(result, red_actions.end());
    red_actions.shrink_to_fit();
    result = remove_if(blue_actions.begin(), blue_actions.end(), [](auto& act){
        return act.apply == 0;
    });
    blue_actions.erase(result, blue_actions.end());
    blue_actions.shrink_to_fit();

    // 一旦 move だけ行う
    move(field, 0, red_actions, actions_buff);
    move(field, 1, blue_actions, actions_buff);

    // removeの処理
    remove(field, 0, red_actions, actions_buff);
    remove(field, 1, blue_actions, actions_buff);

    field.actions = actions_buff;

    return true;
}

bool excludeInvalidActions(const Field& field, int team_index, vector<Action>& actions, vector<Action>& actions_buff)
{
    // 敵のタイルがおかれている場所か
    auto onEnemyTile = [](const vector<vector<int>>& tiled, int enemyTeamID, int y, int x)
    {
        if (tiled[y][x] == enemyTeamID)
        {
            return true;
        }
        return false;
    };
    // タイルが存在しないか
    auto noTile = [](const vector<vector<int>>& tiled, int y, int x)
    {
        if (tiled[y][x] == 0)
        {
            return true;
        }
        return false;
    };

    auto displayInvalid = [](const Agent& agent, const Action& act)
    {
        cout << box::failure << "INVALID" << endl;
        cout << "agentID: " << agent.agentID << endl;
        cout << "agentPos: (" << agent.y << ", " << agent.x << ")" << endl;
        cout << "agentAct: " << act.type  << "(" << act.dy << ", " << act.dx << ")" << endl;
        cout << endl;
    };

    auto result = remove_if(actions.begin(), actions.end(), [&](auto& act){
        if (act.type == "move")
        {
            Team team = field.teams[team_index];
            for (int i = 0; i < team.agents.size(); ++i)
            {
                if (team.agents[i].agentID == act.agentID)
                {
                    int Yed = team.agents[i].y + act.dy;
                    int Xed = team.agents[i].x + act.dx;
                    if (Point(Yed, Xed).isOver(field.height, field.width))
                    {
                        displayInvalid(team.agents[i], act);
                        act.apply = -1;
                        actions_buff.push_back(act);
                        return true;
                    }
                    int rebirth = (team_index == 0) ? 1 : 0;  // 敵チームのインデックス
                    if (onEnemyTile(field.tiled, field.teams[rebirth].teamID, Yed, Xed))
                    {
                        displayInvalid(team.agents[i], act);
                        act.apply = -1;
                        actions_buff.push_back(act);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else if (act.type == "remove")
        {
            Team team = field.teams[team_index];
            for (int i = 0; i < team.agents.size(); ++i)
            {
                if (team.agents[i].agentID == act.agentID)
                {
                    int Yed = field.teams[team_index].agents[i].y + act.dy;
                    int Xed = field.teams[team_index].agents[i].x + act.dx;
                    if (Point(Yed, Xed).isOver(field.height, field.width))
                    {
                        displayInvalid(team.agents[i], act);
                        act.apply = -1;
                        actions_buff.push_back(act);
                        return true;
                    }
                    if (noTile(field.tiled, Yed, Xed))
                    {
                        displayInvalid(team.agents[i], act);
                        act.apply = -1;
                        actions_buff.push_back(act);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    });
    actions.erase(result, actions.end());
    actions.shrink_to_fit();

    return true;
}

bool move(Field& field, int team_index, vector<Action>& actions, vector<Action>& actions_buff)
{
    for (auto& act : actions)
    {
        if (act.type == "move")
        {
            Team team = field.teams[team_index];
            for (int i = 0; i < team.agents.size(); ++i)
            {
                if (team.agents[i].agentID == act.agentID)
                {
                    field.teams[team_index].agents[i].y += act.dy;
                    field.teams[team_index].agents[i].x += act.dx;
                    int ty = field.teams[team_index].agents[i].y;
                    int tx = field.teams[team_index].agents[i].x;
                    field.tiled[ty][tx] = team.teamID;

                    act.apply = 1;
                    actions_buff.push_back(act);
                }
            }
        }
    }
    return true;
}

bool remove(Field& field, int team_index, vector<Action>& actions, vector<Action>& actions_buff)
{
    for (auto& act : actions)
    {
        if (act.type == "remove")
        {
            Team team = field.teams[team_index];
            for (int i = 0; i < team.agents.size(); ++i)
            {
                if (team.agents[i].agentID == act.agentID)
                {
                    int ty = field.teams[team_index].agents[i].y + act.dy;
                    int tx = field.teams[team_index].agents[i].x + act.dx;
                    // 敵のエージェントが除去先に存在しないかの判定
                    for (auto& agent : field.teams[!team_index].agents)
                    {
                        if (agent.y == ty && agent.x == tx)
                        {
                            cout << box::failure << "CONFLICT" << endl;
                            cout << "agentID: " << agent.agentID << endl;
                            cout << "agentPos: (" << agent.y << ", " << agent.x << ")" << endl;
                            cout << "agentAct: " << act.type  << "(" << act.dy << ", " << act.dx << ")" << endl;
                            cout << endl;

                            act.apply = 0;
                            actions_buff.push_back(act);
                        }
                    }
                    // †除去†
                    if (act.apply != 0)
                    {
                        act.apply = 1;
                        actions_buff.push_back(act);
                        field.tiled[ty][tx] = 0;
                    }
                }
            }
        }
    }
    
    return true;
}

bool Log(const Field& field, const vector<Action> red_actions, const vector<Action> blue_actions, int turn)
{
    ofstream ofs("log/turn" + to_string(turn) + ".txt");

    ofs << "[tiled]" << endl;
    for (int y = 0; y < field.height; ++y)
    {
        for (int x = 0; x < field.width; ++x)
        {
            if (field.tiled[y][x] == field.teams[0].teamID)
            {
                ofs << "1 ";
            }
            else if (field.tiled[y][x] == field.teams[1].teamID)
            {
                ofs << "2 ";
            }
            else
            {
                ofs << "0 ";
            }
        }
        ofs << endl;
    }
    ofs << endl;

    // 全置換
    auto ReplaceAll = [](string& replaced, string from, string to)
    {
        auto pos = replaced.find(from);
        int toLen = to.length();
        if (from.empty())
        {
            return replaced;
        }

        while ((pos = replaced.find(from, pos)) != string::npos)
        {
            replaced.replace(pos, from.length(), to);
            pos += toLen;
        }
        return replaced;
    };

    ofs << "[Red Team]" << endl;
    for (const auto& act : red_actions)
    {
        ofs << "agentID: " << act.agentID << endl;
        Point p;
        for (auto& agent : field.teams[0].agents)
        {
            if (agent.agentID == act.agentID)
            {
                for (const auto& field_act : field.actions)
                {
                    if (field_act.apply == 1 && field_act.type == "move")
                    {
                        p.y = agent.y - act.dy;
                        p.x = agent.x - act.dx;
                    }
                    else
                    {
                        p.y = agent.y;
                        p.x = agent.x;
                    }
                }
            }
        }
        ofs << "\t" << "from: (" << p.y << ", " << p.x << ")" << endl;
        ofs << "\t" << "dy: " << act.dy << endl;
        ofs << "\t" << "dx: " << act.dx << endl;
        ofs << "\t" << "type: " << act.type << endl;
        for (const auto& field_act : field.actions)
        {
            if (field_act.agentID == act.agentID)
            {
                ofs << "\t" << "apply: " << field_act.apply << endl;
            }
        }
    }


    ofs << endl;

    ofs << "[Blue Team]" << endl;
    for (const auto& act : blue_actions)
    {
        ofs << "agentID: " << act.agentID << endl;
        Point p;
        for (auto& agent : field.teams[1].agents)
        {
            if (agent.agentID == act.agentID)
            {
                for (const auto& field_act : field.actions)
                {
                    if (field_act.apply == 1 && field_act.type == "move")
                    {
                        p.y = agent.y - act.dy;
                        p.x = agent.x - act.dx;
                    }
                    else
                    {
                        p.y = agent.y;
                        p.x = agent.x;
                    }
                }
            }
        }
        ofs << "\t" << "from: (" << p.y << ", " << p.x << ")" << endl;
        ofs << "\t" << "dy: " << act.dy << endl;
        ofs << "\t" << "dx: " << act.dx << endl;
        ofs << "\t" << "type: " << act.type << endl;
        for (const auto& field_act : field.actions)
        {
            if (field_act.agentID == act.agentID)
            {
                ofs << "\t" << "apply: " << field_act.apply << endl;
            }
        }
    }

    return true;
}

bool Log(string dir_path, const Field& field, const vector<Action> red_actions, const vector<Action> blue_actions, int turn, char red_select, char blue_select)
{
    ofstream ofs(dir_path + "log/turn" + to_string(turn) + ".txt");

    ofs << "[tiled]" << endl;
    for (int y = 0; y < field.height; ++y)
    {
        for (int x = 0; x < field.width; ++x)
        {
            if (field.tiled[y][x] == field.teams[0].teamID)
            {
                ofs << "1 ";
            }
            else if (field.tiled[y][x] == field.teams[1].teamID)
            {
                ofs << "2 ";
            }
            else
            {
                ofs << "0 ";
            }
        }
        ofs << endl;
    }
    ofs << endl;

    // 全置換
    auto ReplaceAll = [](string& replaced, string from, string to)
    {
        auto pos = replaced.find(from);
        int toLen = to.length();
        if (from.empty())
        {
            return replaced;
        }

        while ((pos = replaced.find(from, pos)) != string::npos)
        {
            replaced.replace(pos, from.length(), to);
            pos += toLen;
        }
        return replaced;
    };

    ofs << "[solver]" << endl;
    ofs << "Red : " << red_select << endl;
    ofs << "Blue : " << blue_select << endl;
    ofs << endl;

    ofs << "[Score]" << endl;
    int red_score = ScoreCalculate(field.points, field.normalizedTiled(0));
    int red_tilescore = TileCalculate(field.points, field.normalizedTiled(0));
    int red_areascore = AreaCalculate(field.points, field.normalizedTiled(0));

    int blue_score = ScoreCalculate(field.points, field.normalizedTiled(1));
    int blue_tilescore = TileCalculate(field.points, field.normalizedTiled(1));
    int blue_areascore = AreaCalculate(field.points, field.normalizedTiled(1));
    ofs << "Red : " << red_score << endl;
    ofs << "      " << red_tilescore << "+" << red_areascore << endl;
    ofs << "Blue : " << blue_score << endl;
    ofs << "       " << blue_tilescore << "+" << blue_areascore << endl;
    ofs << endl;

    ofs << "[Red Team]" << endl;
    for (const auto& act : red_actions)
    {
        ofs << "agentID: " << act.agentID << endl;
        Point p;
        for (auto& agent : field.teams[0].agents)
        {
            if (agent.agentID == act.agentID)
            {
                for (const auto& field_act : field.actions)
                {
                    if (field_act.apply == 1 && field_act.type == "move")
                    {
                        p.y = agent.y - act.dy;
                        p.x = agent.x - act.dx;
                    }
                    else
                    {
                        p.y = agent.y;
                        p.x = agent.x;
                    }
                }
            }
        }
        ofs << "\t" << "from: (" << p.y << ", " << p.x << ")" << endl;
        ofs << "\t" << "dy: " << act.dy << endl;
        ofs << "\t" << "dx: " << act.dx << endl;
        ofs << "\t" << "type: " << act.type << endl;
        for (const auto& field_act : field.actions)
        {
            if (field_act.agentID == act.agentID)
            {
                ofs << "\t" << "apply: " << field_act.apply << endl;
            }
        }
    }


    ofs << endl;

    ofs << "[Blue Team]" << endl;
    for (const auto& act : blue_actions)
    {
        ofs << "agentID: " << act.agentID << endl;
        Point p;
        for (auto& agent : field.teams[1].agents)
        {
            if (agent.agentID == act.agentID)
            {
                for (const auto& field_act : field.actions)
                {
                    if (field_act.apply == 1 && field_act.type == "move")
                    {
                        p.y = agent.y - act.dy;
                        p.x = agent.x - act.dx;
                    }
                    else
                    {
                        p.y = agent.y;
                        p.x = agent.x;
                    }
                }
            }
        }
        ofs << "\t" << "from: (" << p.y << ", " << p.x << ")" << endl;
        ofs << "\t" << "dy: " << act.dy << endl;
        ofs << "\t" << "dx: " << act.dx << endl;
        ofs << "\t" << "type: " << act.type << endl;
        for (const auto& field_act : field.actions)
        {
            if (field_act.agentID == act.agentID)
            {
                ofs << "\t" << "apply: " << field_act.apply << endl;
            }
        }
    }

    return true;
}
