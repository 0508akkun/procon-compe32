# pragma once
# include <vector>
# include <queue>
# include <algorithm>
# include <chrono>
# include "State2.h"
# include "Command.h"
# include "Match.h"
# include "Action.h"
# include "Act.h"

/// <summary>
/// 味方全員の最適な一手先を返します
/// </summary>
/// <param name="field">
/// 盤面情報
/// </param>
/// <param name="timelimit_ms">
/// 探索時間制限[ms]
/// </param>
/// <returns>
/// 全員の一手先(Commands)
/// </returns>
Commands beamSearch2(const FFS field, const int timelimit_ms);

/// <summary>
/// beamsearch()を使い適切と思われる次のActionを返します
/// </summary>
std::vector<Action> solve_beam2(const Field& field, const Match& match);