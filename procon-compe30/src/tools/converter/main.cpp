# include "../../search/FieldReader.h"
# include "../../search/ScoreCalculate.h"
# include "../../search/picojson/picojson.h"
# include "../../search/Utility/Box.h"
# include "../../search/Utility/Color.h"
# include "../../search/cmdline.h"
# include <bits/stdc++.h>

using namespace std;

static const string hint = "[" + Color::CharYellow + "!" + Color::Reset + "] ";

cmdline::parser GetOptions(int argc, char* argv[])
{
    cmdline::parser p;
    p.add("atcoder", 'a', "AtCoder風解釈形式からJSON形式に変換します");
    p.add<string>("input", 'i', "入力ファイル名を指定します", false, "path/to/input");
    p.add<string>("output", 'o', "出力ファイル名を指定します", false, "path/to/output");
    p.add("debug", 'd', "ログを出力して実行します");
    p.parse_check(argc, argv);
    return p;
}

int main(int argc, char* argv[])
{
    auto args = GetOptions(argc, argv);
    bool atcoder = args.exist("atcoder");
    bool debug = args.exist("debug");

    if (atcoder)
    {
        string input_path = args.get<string>("input");
        if (args.get<string>("input") == "path/to/input") // パスがデフォルトのままだったら
        {
            cout << box::failure << "ERROR: 入力ファイル名が指定されていません" << endl;
            cout << hint << "オプション引数 [-i] [--input] を追加してください" << endl;
            return 0;
        }

        std::ifstream ifs(input_path);
        if (!ifs) // パスがデフォルトのままだったら
        {
            cout << box::failure << "ERROR: 入力ストリーム生成時エラー" << endl;
            cout << hint << "入力ファイルのパスを確認してください" << endl;
            return 0;
        }

        if (debug)
        {
            cout << box::check << "Convert to JSON ..." << endl;
        }
        std::cin.rdbuf(ifs.rdbuf());

        int N, H, W;
        cin >> N >> H >> W;

        vector<Team> teams;
        vector<Agent> allyAgents;
        vector<Agent> enemyAgents;
        for (int i = 0; i < N; ++i)
        {
            int ys, xs, yr, xr;
            cin >> ys >> xs >> yr >> xr;
            allyAgents.push_back(Agent(i, ys, xs));
            enemyAgents.push_back(Agent(i+10, yr, xr));
        }
        Team allyTeam(6, allyAgents, 0, 0); // teamID, agents, tilePoint, areaPoint
        Team enemyTeam(28, allyAgents, 0, 0); // teamID, agents, tilePoint, areaPoint
        teams.push_back(allyTeam);
        teams.push_back(enemyTeam);

        vector<vector<int>> points(H, vector<int>(W, 0));
        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                int p; cin >> p;
                points[y][x] = p;
            }
        }

        vector<vector<int>> tiled(H, vector<int>(W, 0));
        int  allyTeamID = 6;   // TODO:設定できるようにする
        int enemyTeamID = 28;  // TODO:設定できるようにする
        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                int t; cin >> t;
                switch(t)
                {
                case 0:
                    tiled[y][x] = 0;
                    break;
                case 1:
                    tiled[y][x] = allyTeamID;
                    break;
                case 2:
                    tiled[y][x] = enemyTeamID;
                    break;
                }
            }
        }
        ifs.close();

        vector<Action> actions; // tmp
        
        Field field(H, W, points, 0, 0, tiled, teams, actions); // W, H, points, startedAtUnixTime, turn, tiled, teams, actions
        vector<vector<int>> allyTiled(H, vector<int>(W, 0));
        vector<vector<int>> enemyTiled(H, vector<int>(W, 0));
        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                if (field.tiled[y][x] == 0)
                {
                    allyTiled[y][x] = 0;
                    enemyTiled[y][x] = 0;
                }
                else if (field.tiled[y][x] == allyTeamID)
                {
                    allyTiled[y][x] = 1;
                    enemyTiled[y][x] = 0;
                }
                else if (field.tiled[y][x] == enemyTeamID)
                {
                    allyTiled[y][x] = 0;
                    enemyTiled[y][x] = 1;
                }
            }
        }
        field.teams[0].tilePoint = ScoreCalculate(field.points, allyTiled);
        field.teams[1].tilePoint = ScoreCalculate(field.points, enemyTiled);
        string json = ToJson(field);
        if (debug)
        {
            cout << box::success << "OK!" << endl;
            cout << endl << box::disp << "DISPLAY: field" << endl;
            DisplayField(field, allyTeamID, enemyTeamID);
            cout << endl << box::disp << "DISPLAY: json" << endl;
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
    else
    {
        cout << args.usage() << endl;
    }

    return 0;
}
