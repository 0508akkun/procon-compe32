# pragma once
# include <vector>
# include "Field.h"


/// <summary>
/// 盤面情報からタイルポイントを計算します
/// </summary>
/// <param name="points">
/// 盤面の得点情報
/// </param>
/// <param name="tiles">
/// 塗られているタイルの場所
/// 塗られている   -> true
/// 塗られていない -> false
/// </param>
/// <returns>
/// タイルポイント
/// </returns>
int TileCalculate(const std::vector<std::vector<int>>& points, const std::vector<std::vector<int>>& tiled);

/// <summary>
/// 盤面情報から領域ポイントを計算します
/// </summary>
/// <param name="points">
/// 盤面の得点情報
/// </param>
/// <param name="tiles">
/// 塗られているタイルの場所
/// 塗られている   -> true
/// 塗られていない -> false
/// </param>
/// <returns>
/// 領域ポイント
/// </returns>
int AreaCalculate(const std::vector<std::vector<int>>& points, std::vector<std::vector<int>> tiled);

/// <summary>
/// 盤面情報から得点を計算します
/// </summary>
/// <param name="points">
/// 盤面の得点情報
/// </param>
/// <param name="tiles">
/// 塗られているタイルの場所
/// 塗られている   -> true
/// 塗られていない -> false
/// </param>
/// <returns>
/// 合計得点
/// </returns>
int ScoreCalculate(const std::vector<std::vector<int>>& points, const std::vector<std::vector<int>>& tiled);
int ScoreCalculate(const std::vector<std::vector<int>>& points, const std::vector<std::vector<int>>& tiled,
                          int PointWeight, int AreaWeight);

/// <summary>
/// 盤面情報から敵との得点差を計算します(味方-敵)
/// </summary>
/// <param name="field">
/// 盤面情報
/// <\param>
/// <returns>
/// 得点差
/// 味方が優勢 -> 正
/// 敵が優勢   -> 負
/// </returns>
int LeadScoreCalculate(const FFS& field);
int LeadScoreCalculate(const FFS& field,
                       int allyPointWeight, int allyAreaWeight,
                       int enemyPointWeight, int enemyAreaWeight);

void setupTiles(const FFS& field,
                std::vector<std::vector<int>>& allyTiles, std::vector<std::vector<int>>& enemyTiles);