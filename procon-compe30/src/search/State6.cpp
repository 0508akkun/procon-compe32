# include "State6.h"
# include "Convolution.h"
using namespace std;
using namespace ngng;

State6::State6()
    : via(10)
{
}

State6::State6(FFS _field)
    : field(_field)
    , via(10)
{
    calcScore();
}
 
void State6::calcScore()
{
    score = LeadScoreCalculate(field, 2, 1, 2, 1);
}

/// d : 深さ(0インデックス)
void State6::calcScore(const int d)
{
    const int H = field.height;
    const int W = field.width;

    vector<vector<int>> allyTiled(H, vector<int>(W, 0));
    vector<vector<int>> enemyTiled(H, vector<int>(W, 0));
    for (int y = 0; y < field.height; ++y)
    {
        for (int x = 0; x < field.width; ++x)
        {
            switch (field.Board[y][x].type)
            {
            case ATP::ally:
                allyTiled[y][x] = 1;
                break;
            case ATP::enemy:
                enemyTiled[y][x] = 1;
                break;
            case ATP::newtral:
            default:
                allyTiled[y][x] = 0;
                enemyTiled[y][x] = 0;
            }
        }
    }
    int allyScore = ScoreCalculate(field.points, allyTiled);
    int enemyScore = ScoreCalculate(field.points, enemyTiled);
    int allyTileScore = TileCalculate(field.points, allyTiled);
    int enemyTileScore = TileCalculate(field.points, enemyTiled);
    int allyAreaScore = ScoreCalculate(field.points, allyTiled);
    int enemyAreaScore = ScoreCalculate(field.points, enemyTiled);

    int nearcnt = 1;
    for (int i = 0; i < field.teamnum; i++)
    {
        for (int k = i + 1; k < field.teamnum; k++)
        {
            int threshould_num = 1;
            if(abs(field.Allies[i].pos.x - field.Allies[k].pos.x) <= threshould_num) //近い 
            {
                nearcnt++;
            }
            else if(abs(field.Allies[i].pos.y - field.Allies[k].pos.y) <= threshould_num) //近い
            {
                nearcnt++;
            }
        }
    }

    double rateByNear = (double) 1.0 - (0.05*(nearcnt-1));

    // 評価値
    int evo = 0;
    if (allyTileScore - enemyTileScore > 0)
    {
        evo = (double) rateByNear * (allyTileScore - enemyTileScore);
    }
    else
    {
        evo = allyTileScore - enemyTileScore;
    }

    score = evo;
}

State6 ngng::transition(const State6& state, const int id, const Point dir,
                  const std::vector<std::vector<std::vector<Point>>>& conflictAllyPos,
                  std::vector<std::pair<int,Point>>& deletedTiles,
                  const int d)
{
    State6 next = state;
    const Point here = next.field.Allies[id].pos;
    const int width = next.field.width;
    const int height = next.field.height;
    const Point targetPos = here+dir;
    // 停留措置
    if(dir.isZero())
    {
        Command tmpc;
        next.via[d] = std::make_pair(tmpc,std::make_pair(here,here));
        return next;
    }
    // 場外 // 停留措置
    if(targetPos.isOver(height, width))
    {
        Command tmpc;
        next.via[d] = std::make_pair(tmpc,std::make_pair(here,here));
        return next; 
    }
    // 他人ゾーン // 停留措置
    for(int i=0; i<(int)conflictAllyPos[d].size(); i++)
    {
        if(i==id) continue;
        if(conflictAllyPos[d][i][0] == targetPos || conflictAllyPos[d][i][1] == targetPos)
        {
            Command tmpc;
            next.via[d] = std::make_pair(tmpc,std::make_pair(here,here));
            return next;
        }
    }
    
    ATP& targetType = next.field.Board[targetPos.y][targetPos.x].type;
    // それほんとに消えてますか
    if(targetType != ATP::enemy)
    {
        for(auto p:deletedTiles)
        {
            if(p.second == targetPos && p.first > d)
            {
                targetType = ATP::enemy;
                p.first = d;
            }
        }
    }

    // タイルの色によって行動を決めて返す（本編）
    if(targetType == ATP::enemy)
    {
        deletedTiles.emplace_back(d,targetPos);
        targetType = ATP::newtral;
        Command tmpc(Act::Remove, dir);
        next.via[d] = std::make_pair(tmpc,std::make_pair(here,targetPos));
    }
    else
    {
        targetType = ATP::ally;
        Command tmpc(Act::Move, dir);
        next.via[d] = std::make_pair(tmpc,std::make_pair(here,targetPos));
        next.field.Allies[id] = targetPos;
    }
    next.calcScore(d);
    return next;
}
