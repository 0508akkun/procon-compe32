# include "Action.h"
# include "picojson/picojson.h"

Action::Action(int _agentID, std::string _type, int _dy, int _dx, int _turn, int _apply)
    : agentID(_agentID)
    , type(_type)
    , dy(_dy), dx(_dx)
    , turn(_turn)
    , apply(_apply)
{}

std::string ToJson(const std::vector<Action>& actions)
{
    picojson::object actionsObj;
    picojson::array datalist;

    for (const auto& action : actions)
    {
        picojson::object data;
        data.insert(std::make_pair("agentID", picojson::value((double)action.agentID)));
        data.insert(std::make_pair("dy", picojson::value((double)action.dy)));
        data.insert(std::make_pair("dx", picojson::value((double)action.dx)));
        data.insert(std::make_pair("type", picojson::value(action.type)));
        datalist.push_back(picojson::value(data));
    }

    actionsObj.insert(std::make_pair("actions", picojson::value(datalist)));

    return picojson::value(actionsObj).serialize();
}
