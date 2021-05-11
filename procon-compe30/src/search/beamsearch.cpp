# include "beamsearch.h"
# include "Utility/Box.h"

Commands beamSearch(const FFS field, const int timelimit_ms)
{
    std::chrono::system_clock::time_point start;
    start = std::chrono::system_clock::now();
    const int margin_ms = 500; //猶予時間
    const int looseLimit_ms = timelimit_ms - margin_ms; //猶予のある探索可能時間
    
    const int depth = std::min(10,field.maxturn-field.turn+1); //depth手先まで見る(相手は考えない)

    Commands Cs(field.teamnum);
    // max8人全員同時に動かすと遷移先が爆発するので一人ずつ考える
    // 最初の人の最適解を見つける->そう動いた時の盤面をもとに次の人の最適解を見つける
    const int oneLimit = looseLimit_ms / field.teamnum; // 一人当たりの時間
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
    State tmpState(field);
    std::vector<std::vector<Point>> conflictAllyPos(depth+2);
    for(int i=0;i<field.teamnum;i++)
    {
        conflictAllyPos[0].emplace_back(field.Allies[i].pos);
    }

    for(int i=0;i<field.teamnum;i++) //i人目の手
    {   
        std::cerr << box::check << "THINK: Player No." << i << std::endl;
        std::vector<std::priority_queue<State>> States(depth+2);
        States[0].push(tmpState);
        Command c;
        int whilecounter = 0;
        while(time < oneLimit*(i+1))
        {
            for(int d=0;d<depth;d++)
            {
                if(States[d].empty()) continue;
                State now = States[d].top(); States[d].pop();
                //9通りの方向を与える(行動は一意に定まるので向こうで考える)
                States[d+1].push(transition(now, i, {-1,-1}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, {-1, 0}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, {-1, 1}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, { 0,-1}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, { 0, 0}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, { 0, 1}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, { 1,-1}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, { 1, 0}, conflictAllyPos, d));
                States[d+1].push(transition(now, i, { 1, 1}, conflictAllyPos, d));
            }
            time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
            // just debug
            // std::cout << i << " " << time << std::endl;
            whilecounter++;
        }
        // std::cout << "counter:" << whilecounter << std::endl;
        tmpState = States[depth].top(); //状態遷移
        // 味方が使うので干渉させたくないマスを記録する
        for(int d=0;d<depth;d++)
        {
            for(auto v:tmpState.via)
            {
                conflictAllyPos[d+1].emplace_back(v.second.second);
                if(v.first.a == Act::Remove)
                {
                    conflictAllyPos[d+1].emplace_back(v.second.first);
                }
            }
        }
        c = tmpState.via.at(0).first;
        tmpState.via.clear();
    
        //戻り値
        Cs.cmds[i] = c;
    }
    return Cs;
}

std::vector<Action> solve_beam(const Field& field, const Match& match)
{
    std::vector<Action> acts;
    int actsSize = field.teams[0].agents.size();
    int team_index;

    if(field.teams[0].teamID == match.teamID) team_index = 0;
    else team_index = 1;
    Commands commands = beamSearch(toFFS(field, match.turns, team_index, 
                          field.teams[team_index].teamID, field.teams[!team_index].teamID),
                          match.turnMillis);
    
    assert(field.points.size() == field.height && field.points[0].size() == field.width);
    
    for(int i=0;i<actsSize;i++)
    {
        acts.emplace_back(field.teams[team_index].agents[i].agentID, ActToString(commands.cmds[i].a),
                          commands.cmds[i].to.y, commands.cmds[i].to.x, 0, 1);
    }

    return acts;
}
