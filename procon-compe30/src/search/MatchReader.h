# pragma once
# include "Match.h"
# include "picojson/picojson.h"
# include <vector>

class MatchReader
{
public:
    MatchReader() = default;
    MatchReader(picojson::value val);

    bool writeTo(std::vector<Match>& matches);

private:
    picojson::value init_value;
};

