# include "MatchReader.h"
# include <string>

using namespace std;

MatchReader::MatchReader(picojson::value val)
    : init_value(val)
{
}

bool MatchReader::writeTo(vector<Match>& matches)
{
    matches.clear();
    matches.shrink_to_fit();

    picojson::array& picoArray = init_value.get<picojson::array>();
    for (auto& match : picoArray)
    {
        picojson::object& matchObj = match.get<picojson::object>();
        int id = (int)matchObj["id"].get<double>();
        int intervalMillis = (int)matchObj["intervalMillis"].get<double>();
        string matchTo = matchObj["matchTo"].get<string>();
        int teamID = (int)matchObj["teamID"].get<double>();
        int turnMillis = (int)matchObj["turnMillis"].get<double>();
        int turns = (int)matchObj["turns"].get<double>();
        matches.push_back(Match(id, intervalMillis, matchTo, teamID, turnMillis, turns));
    }

    return true;
}
