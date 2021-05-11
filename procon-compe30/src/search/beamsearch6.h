# pragma once
# include <vector>
# include <queue>
# include <algorithm>
# include <chrono>
# include "State6.h"
# include "Command.h"
# include "Match.h"
# include "Action.h"
# include "Act.h"

namespace ngng
{
    Commands beamsearch6(const FFS field, const int timelimit_ms);

    std::vector<Action> solve_beam6(const Field& field, const Match& match);
}
