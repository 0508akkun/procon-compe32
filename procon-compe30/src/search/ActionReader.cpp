# include "ActionReader.h"
# include <string>
# include <vector>

using namespace std;

ActionReader::ActionReader(picojson::value val)
    : init_value(val)
{
}

bool ActionReader::writeTo(vector<Action>& actions)
{
    actions.clear();
    actions.shrink_to_fit();

    picojson::object& obj = init_value.get<picojson::object>();
    cout << obj["actions"].to_str() << endl;
    for (auto& action : obj["actions"].get<picojson::array>())
    {
        picojson::object& actionObj = action.get<picojson::object>();
        int agentID = (int)actionObj["agentID"].get<double>();
        string type = actionObj["type"].get<string>();
        int dy = (int)actionObj["dy"].get<double>();
        int dx = (int)actionObj["dx"].get<double>();
        int turn = -1;
        int apply = -1;
        actions.push_back(Action(agentID, type, dy, dx, turn, apply));
    }

    return true;
}
