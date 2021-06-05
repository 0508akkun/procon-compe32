# include "FieldReader.h"

using namespace std;

FieldReader::~FieldReader()
{
}

FieldReader::FieldReader(picojson::value val)
    : init_value(val)
{}

bool FieldReader::setPicojsonValue(picojson::value value)
{
    init_value = value;
    return true;
}

bool FieldReader::writeTo(Field& field)
{
    // init_valがpicojson::arrayの時の処理がまだできていない
    picojson::object& obj = init_value.get<picojson::object>();

    int height = (int)obj["height"].get<double>();
    int width = (int)obj["width"].get<double>();

    vector<vector<int>> points(height, vector<int>(width, 0));
    int y, x;
    y = 0;
    for (auto& ps : obj["points"].get<picojson::array>())
    {
        x = 0;
        for (auto& p : ps.get<picojson::array>())
        {
            points[y][x] = (int)p.get<double>();
            ++x;
        }
        ++y;
    }

    int startedAtUnixTime = (int)obj["startedAtUnixTime"].get<double>();
    int turn = (int)obj["turn"].get<double>();

    vector<vector<int>> tiled(height, vector<int>(width, 0));
    y = 0;
    for (auto& tiles : obj["tiled"].get<picojson::array>())
    {
        x = 0;
        for (auto& tile : tiles.get<picojson::array>())
        {
            tiled[y][x] = (int)tile.get<double>();
            ++x;
        }
        ++y;
    }

    vector<Team> teams;
    for (auto& team : obj["teams"].get<picojson::array>())
    {
        picojson::object teamObj = team.get<picojson::object>();
        int teamID = (int)teamObj["teamID"].get<double>();
        vector<Agent> agents;
        for (auto& agent : teamObj["agents"].get<picojson::array>())
        {
            picojson::object agentObj = agent.get<picojson::object>();
            int agentID = (int)agentObj["agentID"].get<double>();
            int y = (int)agentObj["y"].get<double>() - 1;
            int x = (int)agentObj["x"].get<double>() - 1;
            agents.push_back(Agent(agentID, y, x));
        }
        int tilePoint = (int)teamObj["tilePoint"].get<double>();
        int areaPoint = (int)teamObj["areaPoint"].get<double>();
        teams.push_back(Team(teamID, agents, tilePoint, areaPoint));
    }

    vector<Action> actions;
    for (auto& action : obj["actions"].get<picojson::array>())
    {
        picojson::object actionObj = action.get<picojson::object>();
        int agentID = (int)actionObj["agentID"].get<double>();
        string type = actionObj["type"].get<string>();
        int dy = (int)actionObj["dy"].get<double>();
        int dx = (int)actionObj["dx"].get<double>();
        int turn = (int)actionObj["turn"].get<double>();
        int apply = (int)actionObj["apply"].get<double>();
        actions.push_back(Action(agentID, type, dy, dx, turn, apply));
    }

    field = m_field = Field(height, width, points, startedAtUnixTime, turn, tiled, teams, actions);

    return true;
}
