# pragma once
# include <vector>
# include <utility>
# include "Field.h"
# include "Command.h"
# include "Point.h"
# include "ScoreCalculate.h"

namespace ngng
{
    struct State5
    {
        FFS field;
        int score;
        std::vector<std::pair<Command, std::pair<Point,Point>>> via;
        bool operator<(const State5& another) const
        {
            return score < another.score;
        }
        State5();
        State5(FFS);
        void calcScore();
        void calcScore(const int d);
    };


    /// <summary>
    /// ある時点から特定ターン後に味方一人が行動した後の状態を返します
    /// </summary>
    /// <param name="state">
    /// 元の状態
    /// </param>
    /// <param name="id">
    /// 行動するPlayerのインデックス
    /// </param>
    /// <param name="dir">
    /// 行動対象の方向
    /// </param>
    /// <param name="conflictAllyPos">
    /// 味方と行動対象が競合する座標の集合
    /// </param>
    /// <param name="d">
    /// 基点からのターン数競合を参照するターン
    /// </param>
    /// <returns>
    /// 行動した次の状態
    /// </returns>
    State5 transition(const State5& state, const int id, const Point dir,
                      const std::vector<std::vector<std::vector<Point>>>& conflictAllyPos,
                      std::vector<std::pair<int,Point>>& deletedTiles,
                      const int d);
} // namespace ngng
