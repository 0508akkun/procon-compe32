# include "../../search/picojson/picojson.h"
# include "../../search/Utility/Box.h"
# include "../../search/Utility/Color.h"
# include "../../search/cmdline.h"
# include "../../search/ScoreCalculate.h" 
# include "FieldGenerator.h"
# include <bits/stdc++.h>
using namespace std;

static const string hint = "[" + Color::CharYellow + "!" + Color::Reset + "] ";

cmdline::parser GetOptions(int argc, char* argv[])
{
    cmdline::parser p;
    p.add<string>("output", 'o', "出力ファイル名を指定します", false, "path/to/output");
    p.add<int>("height", 'h', "height を指定します", false, 0);
    p.add<int>("width" , 'w', "width を指定します", false, 0);
    p.add("debug", 'd', "ログを出力して実行します");
    p.parse_check(argc, argv);
    return p;
}

enum class SymmetricType
{
    Y_axis,
    X_axis,
    XY_axis,
    Point,
};

void AssignPoints(bool,int, int, int, int, double, vector<vector<int>>&, SymmetricType);
void AssignAgentPosition(int, int, int, int, int, vector<vector<int>>&, vector<vector<int>>&, vector<Agent>&,vector<Agent>&, vector<vector<int>>&, SymmetricType);

int main(int argc, char* argv[])
{
    auto args = GetOptions(argc, argv);
    bool debug = args.exist("debug");

	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<int> RandomFieldSize(10, 20);
	uniform_int_distribution<int> fieldSymmetry(0, 3);
	uniform_int_distribution<int> agentGenerator(2, 8);
	uniform_int_distribution<int> agentID(1, 30);

	bool deviationFlag = false;
	int height = RandomFieldSize(mt);
	int width = RandomFieldSize(mt);
	int agent_num = agentGenerator(mt) * 2;
	int allyTeamID = agentID(mt);
	int enemyTeamID = agentID(mt);
	int minusPercentage = 50;
	double deviation = 5;
	int average = 0;
	vector<vector<int>> points;
	vector<vector<int>> allyTiled;
	vector<vector<int>> enemyTiles;
	vector<Team> teams;
	vector<Agent> allyAgents;
	vector<Agent> enemyAgents;
	vector<vector<int>> tiled;
	vector<Action> actions;

	SymmetricType type = static_cast<SymmetricType>(fieldSymmetry(mt));

	AssignPoints(deviationFlag,height, width, minusPercentage, average, deviation, points, type);
	
	while (allyAgents.size() + enemyAgents.size() != agent_num)
	{
		allyTiled.clear();
		enemyTiles.clear();
		allyAgents.clear();
		enemyAgents.clear();
		tiled.clear();
		AssignAgentPosition(height, width, agent_num, allyTeamID, enemyTeamID, allyTiled, enemyTiles, allyAgents, enemyAgents, tiled, type);
	}
	Team allyTeam(allyTeamID, allyAgents, TileCalculate(points, allyTiled), AreaCalculate(points, allyTiled));
	Team enemyTeam(enemyTeamID, enemyAgents, TileCalculate(points, enemyTiles), AreaCalculate(points, enemyTiles));
	teams.emplace_back(allyTeam);
	teams.emplace_back(enemyTeam);

    Field field(height, width, points, 0, 0, tiled, teams, actions);
    string json = ToJson(field);
    if (debug)
    {
        cout << box::success << "OK!" << endl;
        cout << endl << box::disp << "DISPLAY" << endl;
        cout << json << endl;
        cout << endl;
    }

    string output_path = args.get<string>("output");
    if (output_path == "path/to/output")
    {
        output_path = "./result.json";
    }
    if (debug)
    {
        cout << box::check << "output \"" << output_path << "\"" << endl;
    }

    ofstream ofs(output_path);
    if (!ofs)
    {
        cout << box::failure << "ERROR: 出力ストリーム生成時にエラーが発生しました" << "\"" << endl;
    }
    ofs << json;
    if (debug)
    {
        cout << box::success << "OK!" << endl;
    }

    return 0;
}

void AssignPoints(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points, SymmetricType type)
{
	switch (type)
	{
	case SymmetricType::Y_axis:
		GenerateSymmetricY_axisField(deviationFlag,height, width, minusPercentage, average, deviation, points);
		break;
	case SymmetricType::X_axis:
		GenerateSymmetricX_axisField(deviationFlag,height, width, minusPercentage, average, deviation, points);
		break;
	case SymmetricType::XY_axis:
		GenerateSymmetricXY_axisField(deviationFlag, height, width, minusPercentage, average, deviation, points);
		break;
	case SymmetricType::Point:
		GenerateSymmetricPointField(deviationFlag, height, width, minusPercentage, average, deviation, points);
		break;
	}
}

void AssignAgentPosition(int height, int width, int agentNum, int allyTeamID, int enemyTeamID,vector<vector<int>>& allyTiled, vector<vector<int>>& enemyTiles, vector<Agent> &allyAgents, vector<Agent> &enemyAgents, vector<vector<int>>& tiled, SymmetricType type)
{
	int agentCount = 0;
	switch (type)
	{
	case SymmetricType::Y_axis:
		AssignSymmetricY_axisAgent(height, width, agentNum, allyTiled, enemyTiles, allyAgents, enemyAgents);
		break;
	case SymmetricType::X_axis:
		AssignSymmetricX_axisAgent(height, width, agentNum, allyTiled, enemyTiles, allyAgents, enemyAgents);
		break;
	case SymmetricType::XY_axis:
		AssignSymmetricPointAgent(height, width, agentNum, allyTiled, enemyTiles, allyAgents, enemyAgents);
		break;
	case SymmetricType::Point:
		AssignSymmetricPointAgent(height, width, agentNum, allyTiled, enemyTiles, allyAgents, enemyAgents);
		break;
	}
	AssignTiled(allyTeamID, enemyTeamID, allyTiled, enemyTiles, tiled);
}