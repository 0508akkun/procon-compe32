# include "State7.h"
# include "Convolution.h"
using namespace std;
using namespace pointn;

State7::State7()
    : via(10)
    , pointList(11, vector<int>(8, 0))
{
}

State7::State7(FFS _field)
    : field(_field)
    , via(10)
    , pointList(11, vector<int>(8, 0))
{
    calcScore();
}
 
void State7::calcScore()
{
    score = LeadScoreCalculate(field, 3, 1, 2, 2);
}

void State7::calcScores(const int d, const int left)
{
    // pointListの更新 
    std::vector<std::vector<int>> allyTiled(field.height, std::vector<int>(field.width));
    std::vector<std::vector<int>> enemyTiled(field.height, std::vector<int>(field.width));
    setupTiles(field, allyTiled, enemyTiled);
    pointList[d][0] = TileCalculate(field.points, allyTiled);
    pointList[d][1] = AreaCalculate(field.points, allyTiled);
    pointList[d][2] = TileCalculate(field.points, enemyTiled);
    pointList[d][3] = AreaCalculate(field.points, enemyTiled);
    pointList[d][4] = pointList[d][0] + pointList[d][1];
    pointList[d][5] = pointList[d][2] + pointList[d][3];
    pointList[d][6] = pointList[d][4] - pointList[d][5];
    pointList[d][7] = pointList[d][0]*3+pointList[d][1]-(pointList[d][2]*2+pointList[d][3]*2);
    score = pointList[d][7];
}

State7 pointn::transition(const State7& state, const int id, const Point dir,
                  const std::vector<std::vector<std::vector<Point>>>& conflictAllyPos,
                  std::vector<std::pair<int,Point>>& deletedTiles,
                  const int d, const int left)
{
    State7 next = state;
    const Point here = next.field.Allies[id].pos;
    const int width = next.field.width;
    const int height = next.field.height;
    const Point targetPos = here+dir;
    for(int i=0;i<(int)next.pointList[0].size();i++)
    {
        next.pointList[d+1][i] = next.pointList[d][i];
    }
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
    std::random_device rnd;
    // 敵ゾーン
    for(auto e:next.field.Enemies){
        if(e.pos == targetPos && (rnd()&1)){
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
    next.calcScores(d+1, left);
    return next;
}
