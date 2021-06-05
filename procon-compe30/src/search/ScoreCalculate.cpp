# include "ScoreCalculate.h" 
# include "Utility/Array.h"
# include <cstdlib>
using namespace std;

bool OverField(const vector<vector<int>>& tiled, int y, int x)
{
    if (y < 0 || tiled.size() - 1 < y)
    {
        return true;
    }
    if (x < 0 || tiled[0].size() - 1 < x)
    {
        return true;
    }
    return false;
}

// notSearchedNum
// 未探索とする数字
// 例えば初めての段階では 0 が未探索を示すため notSearchedNum は 0
void areaSearch(vector<vector<int>>& tiled, int y, int x, int index, int notSearchedNum = 0)
{
    int dx[4] = { 1, -1, 0,  0 };
    int dy[4] = { 0,  0, 1, -1 };
    tiled[y][x] = index;
    for (int i = 0; i < 4; ++i)
    {
        if (!OverField(tiled, y + dy[i], x + dx[i]))
        {
            if (tiled[y + dy[i]][x + dx[i]] == notSearchedNum /*&& !(tiled[y+dy[i]][x+dx[i]] < index)*/)
            {
                tiled[y + dy[i]][x + dx[i]] = index;
                areaSearch(tiled, y + dy[i], x + dx[i], index, notSearchedNum);
            }
        }
    }
}

// タイルポイントの計算
int TileCalculate(const vector<vector<int>>& points, const vector<vector<int>>& tiled)
{
    int sum = 0;
    for (int y = 0; y < points.size(); ++y)
    {
        for (int x = 0; x < points[0].size(); ++x)
        {
            if (tiled[y][x] == true)
            {
                sum += points[y][x];
            }
        }
    }

    return sum;
}

int AreaCalculate(const vector<vector<int>>& points, vector<vector<int>> tiled)
{
    // 領域ポイントの計算
    // イメージ : 4近傍に水を流す
    int index = 2;
    for (int y = 0; y < points.size(); ++y)
    {
        for (int x = 0; x < points[0].size(); ++x)
        {
            if (tiled[y][x] == 0)
            {
                areaSearch(tiled, y, x, index);
                index++;
            }
        }
    }

    // 角および辺は囲い判定対象外なので、それを除外する。
    for (int y = 0; y < points.size(); ++y)
    {
        if (tiled[y][0] != 1 && tiled[y][0] != 0)
        {
            areaSearch(tiled, y, 0, 0, tiled[y][0]);
        }
        if (tiled[y][tiled[0].size() - 1] != 1 && tiled[y][tiled[0].size() - 1] != 0)
        {
            areaSearch(tiled, y, tiled[0].size() - 1, 0, tiled[y][tiled[0].size() - 1]);
        }
    }
    for (int x = 0; x < points[0].size(); ++x)
    {
        if (tiled[0][x] != 1 && tiled[0][x] != 0)
        {
            areaSearch(tiled, 0, x, 0, tiled[0][x]);
        }
        if (tiled[tiled.size() - 1][x] != 1 && tiled[tiled.size() - 1][x] != 0)
        {
            areaSearch(tiled, tiled.size() - 1, x, 0, tiled[tiled.size() - 1][x]);
        }
    }

    // 囲われているところ = 数字が 1 を超えている(2 以上の)タイル
    int sum = 0;
    for (int y = 0; y < points.size(); ++y)
    {
        for (int x = 0; x < points[0].size(); ++x)
        {
            if (tiled[y][x] >= 2)
            {
                sum += std::abs(points[y][x]);
            }
        }
    }

    return sum;
}

int ScoreCalculate(const vector<vector<int>>& points, const vector<vector<int>>& tiled)
{
    return TileCalculate(points, tiled) + AreaCalculate(points, tiled);
}

int LeadScoreCalculate(const FFS& field)
{
    vector<vector<int>> allyTiles(field.height,vector<int>(field.width));
    vector<vector<int>> enemyTiles(field.height,vector<int>(field.width));
    setupTiles(field, allyTiles, enemyTiles);
    return ScoreCalculate(field.points, allyTiles) - ScoreCalculate(field.points, enemyTiles);
}

int ScoreCalculate(const vector<vector<int>>& points, const vector<vector<int>>& tiled,
                   int PointWeight, int AreaWeight)
{
    return TileCalculate(points, tiled) * PointWeight + AreaCalculate(points, tiled) * AreaWeight;
}
int LeadScoreCalculate(const FFS& field,
                       int allyPointWeight, int allyAreaWeight,
                       int enemyPointWeight, int enemyAreaWeight)
{
    vector<vector<int>> allyTiles(field.height,vector<int>(field.width));
    vector<vector<int>> enemyTiles(field.height,vector<int>(field.width));
    setupTiles(field, allyTiles, enemyTiles);
    return ScoreCalculate(field.points, allyTiles, allyPointWeight, allyAreaWeight) 
            - ScoreCalculate(field.points, enemyTiles, enemyPointWeight, enemyAreaWeight);
}

void setupTiles(const FFS& field, vector<vector<int>>& allyTiles, vector<vector<int>>& enemyTiles)
{
    for(int y=0; y < field.height; y++)
    {
        for(int x=0; x < field.width; x++)
        {
            if(field.Board[y][x].type == ATP::ally)
            {
                allyTiles[y][x] = 1;
                enemyTiles[y][x] = 0;
            }
            else if(field.Board[y][x].type == ATP::enemy)
            {
                allyTiles[y][x] = 0;
                enemyTiles[y][x] = 1;
            }
            else
            {
                allyTiles[y][x] = 0;
                enemyTiles[y][x] = 0;
            }
        }
    }
}