# include "beamsearch4.h"
# include "Utility/Box.h"

Commands beamsearch4(const FFS field, const int timelimit_ms)
{
    std::chrono::system_clock::time_point start;
    start = std::chrono::system_clock::now();
    const int margin_ms = 500; //猶予時間
    const int looseLimit_ms = timelimit_ms - margin_ms; //猶予のある探索可能時間
    
    const int depth = std::min(10,field.maxturn-field.turn+1); //depth手先まで見る(相手は考えない)
    std::cout << "depth:" << depth << std::endl;
    Commands Cs(field.teamnum);
    const int oneLimit = looseLimit_ms / field.teamnum; // 一人当たりの時間
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
    State4 tmpState(field);
    tmpState.calcScores(0, depth);
    
    std::vector<std::vector<std::vector<Point>>> conflictAllyPos
        (depth+2, std::vector<std::vector<Point>>(field.teamnum,std::vector<Point>(2, {-1,-1}))); //危ない初期化
    std::vector<std::pair<int,Point>> deletedTiles;
    // max8人全員同時に動かすと遷移先が爆発するので一人ずつ考える
    // 最初の人の最適解を見つける->そう動いた時の盤面をもとに次の人の最適解を見つける
    for(int i=0;i<field.teamnum;i++)
    {
        conflictAllyPos[0][i][0] = (field.Allies[i].pos);
    }

    for(int i=0;i<field.teamnum;i++) //i人目の手
    {   
        std::cerr << box::check << "THINK: Player No." << i << std::endl;
        std::vector<std::priority_queue<State4>> States(depth+2);
        States[0].push(tmpState);
        Command c;
        int whilecounter = 0;
        while(time < oneLimit*(i+1))
        {
            for(int d=0;d<depth;d++)
            {
                if(States[d].empty()) continue;
                State4 now = States[d].top(); States[d].pop();
                //9通りの方向を与える(行動は一意に定まるので向こうで考える)
                States[d+1].push(transition(now, i, {-1,-1}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, {-1, 0}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, {-1, 1}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, { 0,-1}, conflictAllyPos, deletedTiles, d, depth));
                // States[d+1].push(transition(now, i, { 0, 0}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, { 0, 1}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, { 1,-1}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, { 1, 0}, conflictAllyPos, deletedTiles, d, depth));
                States[d+1].push(transition(now, i, { 1, 1}, conflictAllyPos, deletedTiles, d, depth));
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
            if(tmpState.via[d].first.a == Act::Move)
            {
                conflictAllyPos[d][i][0] = {-1, -1}; //危ない
                conflictAllyPos[d][i][1] = tmpState.via[d].second.second;
            }
            else
            {
                conflictAllyPos[d][i][0] = tmpState.via[d].second.first;
                conflictAllyPos[d][i][1] = tmpState.via[d].second.second;
            }
        }
        c = tmpState.via.at(0).first;

        // pointListのデバッグ
        std::cout << "scores:";
        for(int i=0;i<(int)tmpState.pointList.size();i++){
            std::cout << tmpState.pointList[i][6] << " ";
        }
        std::cout << std::endl;

        // 次への準備
        for(int y=0;y<(int)tmpState.pointList.size();y++){
            for(int x=0;x<(int)tmpState.pointList[0].size();x++){
                if(y==0) tmpState.pointList[y][x] = tmpState.pointList[depth][x];
                else tmpState.pointList[y][x] = 0;
            }
        }

        //戻り値
        Cs.cmds[i] = c;
    }
    // beamsearch()が使用した時間
    std::cout << "time:" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << "ms" << std::endl;
    return Cs;
}

std::vector<Action> solve_beam4(const Field& field, const Match& match)
{
    std::vector<Action> acts;
    int actsSize = field.teams[0].agents.size();
    int team_index;

    if(field.teams[0].teamID == match.teamID) team_index = 0;
    else team_index = 1;
    Commands commands = beamsearch4(toFFS(field, match.turns, team_index, 
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
