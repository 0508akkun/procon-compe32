# pragma once
# include <bits/stdc++.h>
# include "Player.h"
# include "Cell.h"
# include "Match.h"
# include "Team.h"
# include "Action.h"
# include "Point.h"
using UnixTime = long;

struct FieldForSearch
{
    int width,height;
    int maxturn;
    int turn;
    std::vector<std::vector<Cell>> Board;
    static std::vector<std::vector<int>> points;
    std::vector<Player> Enemies;
    std::vector<Player> Allies;
    int teamnum;

    FieldForSearch();
    FieldForSearch(int,int,int,int,std::vector<std::vector<Cell>>,
          std::vector<Player>, std::vector<Player>);
    FieldForSearch(int,int,int);
    FieldForSearch(int,int,int,int,int);
    static void reset();
};

using FFS = FieldForSearch;


// これ複数人と対戦するときどうなるんだ…？

struct Field
{
    Field() = default;
    Field(int height, int width, 
        const std::vector<std::vector<int>>& _points,
        UnixTime _startedAtUnixTime,
        int _turn,
        const std::vector<std::vector<int>>& _tiled,
        const std::vector<Team>& _teams,
        const std::vector<Action>& _actions
    );
    int height;
    int width;
    std::vector<std::vector<int>> points;
    UnixTime startedAtUnixTime; // Fieldとは無関係なのでメンバに含ませるかは審議
    int turn;
    std::vector<std::vector<int>> tiled;
    std::vector<Team> teams; 
    std::vector<Action> actions;

    /// <summary>
    /// 正規化したフィールドを返します
    /// </summary>
    std::vector<std::vector<int>> normalizedTiled(int team_index) const;

    /// <summar>
    /// Ally と Enemy の team_index を返します
    /// </summar>
    /// <param name="match">
    /// 試合情報
    /// </summar>
    /// <returns>
    /// first : ally_index  ( 0 | 1 )
    /// second: enemy_index ( 1 | 0 )
    /// </return>
    std::pair<int, int> getIndex(const Match& match) const;

    /// <summar>
    /// Ally と Enemy の team_index を返します
    /// </summar>
    /// <param name="ally_teamID">
    /// 味方のteamID
    /// </summar>
    /// <returns>
    /// first : ally_index  ( 0 | 1 )
    /// second: enemy_index ( 1 | 0 )
    /// </return>
    std::pair<int, int> getIndex(int ally_teamID) const;
};

/// <summary>
/// フィールドを表示します。
/// </summary>
/// <param name="points">
/// 盤面の得点情報
/// </param>
/// <remarks>
/// ANSIエスケープシーケンス対応が条件
/// </remarks>
bool DisplayField(
        const std::vector<std::vector<int>>& points,
        const std::vector<std::vector<int>>& allyTiles,
        const std::vector<std::vector<int>>& enemyTiles,
        const std::vector<Point>& agentPos
        );

/// <summary>
/// フィールドを表示します。
/// </summary>
/// <param name="field">
/// 盤面の情報
/// </param>
/// <remarks>
/// ANSIエスケープシーケンス対応が条件
/// </remarks>
bool DisplayField(const Field& field, int allyTeamID, int enemyTeamID);

/// <summary>
/// フィールドのデータを表示します。
/// </summary>
/// <param name="field">
/// フィールド
/// </param>
bool DisplayFieldData(const Field& field);

/// <summary>
/// フィールドのデータをJSON形式に変換します
/// </summary>
/// <param name="field">
/// フィールド
/// </param>
/// <returns>
/// JSON形式に変換されたフィールド
/// </returns>
std::string ToJson(const Field& field);

/// <summary>
/// フィールドのデータを Log として出力します。
/// </summary>
/// <param name="field">
/// フィールド
/// </param>
/// <param name="path">
/// 出力先のファイル名
/// </param>
bool LogField(const Field& field, std::string path);

/// <summary>
/// FieldをFFSに変換します
/// </summary>
FFS toFFS(const Field& field, const int turns, const int team_index, const int allyID, const int enemyID);
