# include "Field.h"
# include "Utility/Color.h"
# include "Utility/Array.h"
# include "picojson/picojson.h"
# include "Utility/Box.h"
# include <bits/stdc++.h>
using namespace std;

vector<vector<int>> FFS::points;

FFS::FieldForSearch()
{
}

FFS::FieldForSearch(int w, int h,
             int mt, int t,
             std::vector<std::vector<Cell>> b,
             std::vector<Player> e, std::vector<Player> a)
             : width(w), height(h)
             , maxturn(mt), turn(t)
             , Board(b)
             , Enemies(e), Allies(a)
{
    teamnum = (int)Allies.size();
}

FFS::FieldForSearch(int w, int h, int n)
     : width(w), height(h)
     , maxturn(30), turn(1)
     , teamnum(n)
{
    Board.resize(h,std::vector<Cell>(w));
    Enemies.resize(n);
    Allies.resize(n);
}

FFS::FieldForSearch(int w, int h, int n, int mt, int t)
     : width(w), height(h)
     , maxturn(mt), turn(t)
     , teamnum(n)
{
    Board.resize(h,std::vector<Cell>(w));
    Enemies.resize(n);
    Allies.resize(n);
}

void FFS::reset(){
    for(int i=0;i<points.size();i++){
        points[i].clear();
        points[i].shrink_to_fit();
    }
    points.clear();
    points.shrink_to_fit();
}

Field::Field(int _height, int _width, 
    const std::vector<std::vector<int>>& _points,
    UnixTime _startedAtUnixTime,
    int _turn,
    const std::vector<std::vector<int>>& _tiled,
    const std::vector<Team>& _teams,
    const std::vector<Action>& _actions
)
    : height(_height)
    , width(_width)
    , points(_points)
    , startedAtUnixTime(_startedAtUnixTime)
    , turn(_turn)
    , tiled(_tiled)
    , teams(_teams)
    , actions(_actions)
{}

vector<vector<int>> Field::normalizedTiled(int team_index) const
{
    vector<vector<int>> normalizeTiled(height, vector<int>(width, 0));
    int teamID = teams[team_index].teamID;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            normalizeTiled[y][x] = (tiled[y][x] == teamID);
        }
    }

    return normalizeTiled;
}

pair<int, int> Field::getIndex(const Match& match) const
{
    if (teams.at(0).teamID == match.teamID)
    {
        return pair<int, int>(0, 1);
    }
    else
    {
        return pair<int, int>(1, 0);
    }
}

pair<int, int> Field::getIndex(int ally_teamID) const
{
    if (teams.at(0).teamID == ally_teamID)
    {
        return pair<int, int>(0, 1);
    }
    else
    {
        return pair<int, int>(1, 0);
    }
}

bool DisplayField(
        const vector<vector<int>>& points,
        const vector<vector<int>>& allyTiles,
        const vector<vector<int>>& enemyTiles,
        const std::vector<Point>& agentPos
        )
{

    for (int y = 0; y < points.size(); ++y)
    {
        for (int x = 0; x < points[0].size(); ++x)
        {
            for (int agent = 0; agent < agentPos.size(); ++agent)
            {
                if (y == agentPos[agent].y && x == agentPos[agent].x)
                {
                    printf("%s", Color::CharYellow.c_str());
                    break;
                }
            }
            if (allyTiles[y][x] == 1)
            {
                printf("%s", Color::BackRed.c_str());
            }
            else if (enemyTiles[y][x] == 1)
            {
                printf("%s", Color::BackCyan.c_str());
            }
            printf("%3d%s ", points[y][x], Color::Reset.c_str());
        }
        printf("\n");
    }

    return true;
}

bool DisplayField(const Field& field, int allyTeamID, int enemyTeamID)
{
    for (int y = 0; y < field.height; ++y)
    {
        for (int x = 0; x < field.width; ++x)
        {
            for (int team = 0; team < field.teams.size(); ++team)
            {
                for (int agent = 0; agent < field.teams[team].agents.size(); ++agent)
                {
                    if (y == field.teams[team].agents[agent].y && x == field.teams[team].agents[agent].x)
                    {
                        printf("%s", Color::CharYellow.c_str());
                        break;
                    }
                }
            }
            if (field.tiled[y][x] == allyTeamID)
            {
                printf("%s", Color::BackRed.c_str());
            }
            if (field.tiled[y][x] == enemyTeamID)
            {
                printf("%s", Color::BackCyan.c_str());
            }
            printf("%3d%s ", field.points[y][x], Color::Reset.c_str());
        }
        printf("\n");
    }

    return true;
}

bool DisplayFieldData(const Field& field)
{
    cout << "height : " << field.height << "\n";
    cout << "width : " << field.width << "\n";

    cout << "[\n";
    for (int y = 0; y < field.height; ++y)
    {
        cout << "  ";
        for (int x = 0; x < field.width; ++x)
        {
            printf("%2d ", field.points[y][x]);
        }
        cout << "\n";
    }
    cout << "]\n";
    
    cout << "startedAtUnixTime : " << field.startedAtUnixTime << "\n";
    cout << "turn : " << field.turn << "\n";

    cout << "[\n";
    for (int y = 0; y < field.height; ++y)
    {
        cout << "  ";
        for (int x = 0; x < field.width; ++x)
        {
            printf("%2d ", field.tiled[y][x]);
        }
        cout << "\n";
    }
    cout << "]\n";

    for (int i = 0; i < field.teams.size(); ++i)
    {
        cout << "teams[" << i << "].teamID : " << field.teams[i].teamID << "\n";
        for (int k = 0; k < field.teams[i].agents.size(); ++k)
        {
            cout << "teams[" << i << "]." << "agents[" << k << "].agentID : " << field.teams[i].agents[k].agentID << "\n";
            cout << "teams[" << i << "]." << "agents[" << k << "].y : " << field.teams[i].agents[k].y << "\n";
            cout << "teams[" << i << "]." << "agents[" << k << "].x : " << field.teams[i].agents[k].x << "\n";
        }
        cout << "teams[" << i << "].tilePoint : " << field.teams[i].tilePoint << "\n";
        cout << "teams[" << i << "].areaPoint : " << field.teams[i].areaPoint << "\n";
    }
    for (int i = 0; i < field.actions.size(); ++i)
    {
        cout << "actions[" << i << "].agentID: " << field.actions[i].agentID << "\n";
        cout << "actions[" << i << "].type: " << field.actions[i].type << "\n";
        cout << "actions[" << i << "].dy: " << field.actions[i].dy << "\n";
        cout << "actions[" << i << "].dx: " << field.actions[i].dx << "\n";
        cout << "actions[" << i << "].turn: " << field.actions[i].turn << "\n";
        cout << "actions[" << i << "].apply: " << field.actions[i].apply << "\n";
    }

    return true;
}

string ToJson(const Field& field)
{
    picojson::object fieldObj;

    fieldObj.insert(make_pair("height", picojson::value((double)field.height)));
    fieldObj.insert(make_pair("width", picojson::value((double)field.width)));

    picojson::array pointsData;
    for (int y = 0; y < field.height; ++y)
    {
        picojson::array pointsArray;
        for (int x = 0; x < field.width; ++x)
        {
            pointsArray.push_back(picojson::value((double)field.points[y][x]));
        }
        pointsData.push_back(picojson::value(pointsArray));
    }
    fieldObj.insert(make_pair("points", picojson::value(pointsData)));

    fieldObj.insert(make_pair("startedAtUnixTime", picojson::value((double)field.startedAtUnixTime)));
    fieldObj.insert(make_pair("turn", picojson::value((double)field.turn)));

    picojson::array tiledData;
    for (int y = 0; y < field.height; ++y)
    {
        picojson::array tiledArray;
        for (int x = 0; x < field.width; ++x)
        {
            tiledData.push_back(picojson::value((double)field.tiled[y][x]));
        }
        pointsData.push_back(picojson::value(tiledArray));
    }
    fieldObj.insert(make_pair("tiled", picojson::value(tiledData)));

    picojson::array teamsList;
    for (auto& team : field.teams)
    {
        picojson::object data;
        data.insert(make_pair("teamID", picojson::value((double)team.teamID)));
        picojson::array agentsList;
        for (auto& agent : team.agents)
        {
            picojson::object agentData;
            agentData.insert(make_pair("agentID", picojson::value((double)agent.agentID)));
            agentData.insert(make_pair("y", picojson::value((double)agent.y)));
            agentData.insert(make_pair("x", picojson::value((double)agent.x)));
            agentsList.push_back(picojson::value(agentData));
        }
        data.insert(make_pair("agents", picojson::value(teamsList)));
        data.insert(make_pair("tilePoint", picojson::value((double)team.tilePoint)));
        data.insert(make_pair("areaPoint", picojson::value((double)team.areaPoint)));
        teamsList.push_back(picojson::value(data));
    }
    fieldObj.insert(make_pair("teams", picojson::value(teamsList)));

    picojson::array actionsList;
    for (const auto& action : field.actions)
    {
        picojson::object data;
        data.insert(std::make_pair("agentID", picojson::value((double)action.agentID)));
        data.insert(std::make_pair("dy", picojson::value((double)action.dy)));
        data.insert(std::make_pair("dx", picojson::value((double)action.dx)));
        data.insert(std::make_pair("type", picojson::value(action.type)));
        actionsList.push_back(picojson::value(data));
    }
    fieldObj.insert(std::make_pair("actions", picojson::value(actionsList)));

    return picojson::value(fieldObj).serialize();
}

bool LogField(const Field& field, string path)
{
    ofstream ofs;
    try
    {
        ofs.exceptions(ofstream::failbit);
        ofs.open(path.c_str());
    }
    catch (const std::ofstream::failure event)
    {
        cout << box::failure << "ERROR: " << "入力ストリームエラー" << std::endl;
        cout << box::hint << "HINT: " << "log/ ディレクトリは手動で生成していますか？" << std::endl;
    }

    ofs << "[Size]" << endl;
    ofs << "height: " << field.height << "\n";
    ofs << "width : " << field.width << "\n";
    ofs << "\n";

    ofs << "[Points]" << endl;
    ofs << "points: [\n";
    for (int y = 0; y < field.height; ++y)
    {
        ofs << "  ";
        for (int x = 0; x < field.width; ++x)
        {
            ofs << setw(3) << field.points[y][x] << " ";
        }
        ofs << "\n";
    }
    ofs << "]\n";

    ofs << "\n";
    
    ofs << "[Tiled]" << endl;
    ofs << "tiled: [\n";
    for (int y = 0; y < field.height; ++y)
    {
        ofs << "  ";
        for (int x = 0; x < field.width; ++x)
        {
            ofs << setw(2) << field.tiled[y][x] << " ";
        }
        ofs << "\n";
    }
    ofs << "]\n";

    ofs << "\n";

    ofs << "[Turn]" << "\n";
    ofs << "turn: " << field.turn << "\n";

    ofs << "\n";

    ofs << "[Teams]" << "\n";
    for (int i = 0; i < field.teams.size(); ++i)
    {
        ofs << "teams[" << i << "].teamID: " << field.teams[i].teamID << "\n";
        for (int k = 0; k < field.teams[i].agents.size(); ++k)
        {
            ofs << "\t" << "agents[" << k << "].agentID: " << field.teams[i].agents[k].agentID << "\n";
            ofs << "\t" << "agents[" << k << "].y: " << field.teams[i].agents[k].y << "\n";
            ofs << "\t" << "agents[" << k << "].x: " << field.teams[i].agents[k].x << "\n";
            ofs << "\n";
        }
        ofs << "\ttilePoint: " << field.teams[i].tilePoint << "\n";
        ofs << "\tareaPoint: " << field.teams[i].areaPoint << "\n";

        ofs << "\n";
    }

    ofs << "\n";

    for (int i = 0; i < field.actions.size(); ++i)
    {
        ofs << "actions[" << i << "].agentID: " << field.actions[i].agentID << "\n";
        ofs << "actions[" << i << "].type: " << field.actions[i].type << "\n";
        ofs << "actions[" << i << "].dy: " << field.actions[i].dy << "\n";
        ofs << "actions[" << i << "].dx: " << field.actions[i].dx << "\n";
        ofs << "actions[" << i << "].turn: " << field.actions[i].turn << "\n";
        ofs << "actions[" << i << "].apply: " << field.actions[i].apply << "\n";
    }

    ofs << "[StartedAtUnixTime]" << "\n";
    ofs << "startedAtUnixTime: " << field.startedAtUnixTime << "\n";

    ofs.close();

    return true;
}

FFS toFFS(const Field& field, const int turns, const int team_index, const int allyID, const int enemyID)
{
    FFS ffs
    (
        field.width,
        field.height,
        field.teams[0].agents.size(),
        turns,
        field.turn
    );
    ffs.points = field.points;
    for(int y=0;y<ffs.height;y++)
    {
        for(int x=0;x<ffs.width;x++)
        {
            if(field.tiled[y][x] == allyID)
                ffs.Board[y][x].type = ATP::ally;
            else if(field.tiled[y][x] == enemyID)
                ffs.Board[y][x].type = ATP::enemy;
            else
                ffs.Board[y][x].type = ATP::newtral;
        }
    }
    for(int i=0;i<ffs.teamnum;i++){
        ffs.Allies[i].pos = {field.teams[team_index].agents[i].y, field.teams[team_index].agents[i].x};
        ffs.Enemies[i].pos = {field.teams[!team_index].agents[i].y, field.teams[!team_index].agents[i].x};
    }
    return ffs;
}
