# include "Battle.h"
# include "AccessServer.h"
# include "Utility/Box.h"
# include "Field.h"
# include "FieldReader.h"
# include "Match.h"
# include "beamsearch7.h"
# include "OptionArg.h"
# include "picojson/picojson.h"
# include "OptionArg.h"
# include <bits/stdc++.h>
# include <sys/time.h>

using namespace std;
void Battle(const Match& match,  bool debug, int error_count)
{
    int matchID = match.id;

    std::string url = OptionArg::GetURLInfo();

    cout << box::check << "GET: " << url + "matches/" << matchID << endl;
    Response res = GetStatus(matchID,url);
    bool result = CheckStatus(res, matchID);
    if (debug)
    {
        cout << endl << box::disp << "DISPLAY: status" << endl;
        cout << res.body << endl;
    }
    if (!result)
    {
        if (res.body == "TooEarly")
        {
            if (error_count >= 100)
            {
                cout << box::failure << "Too Early So Please Retry (matchID: " << matchID << ")" << endl;
                return;
            }
                cout << box::failure << "なんか数字を入力してからエンターを押すとリスタートする！" << endl;
                int sdfpisajf;
                cin >> sdfpisajf;
            error_count++;
            Battle(match, debug, error_count);
            return;
        }
    }

    Field field;
    vector<Action> actions;
    picojson::value val;
    picojson::parse(val, res.body);
    FieldReader fieldReader(val);
    fieldReader.writeTo(field);


    std::chrono::system_clock::time_point start, end;
    while(1)
    {
        field.turn+=1;
        int turn = field.turn;
        start = std::chrono::system_clock::now();

        cout << box::check << "RUN: solve()" << endl;

        // TODO: ソルバ関数を挿入
        actions = pointn::solve_beam7(field, match);

        cout << box::check << "solved! (matchID: " << matchID << ")" << endl;

        string actions_json = ToJson(actions);
            
        if (debug)
        {
            cout << endl << box::disp << "DISPLAY: actions" << endl;
            cout << actions_json << endl;
        }

        cout << box::check << "POST: " << url + "matches/" << matchID << endl;
        res = PostActions(actions_json, matchID,url);
        CheckStatus(res, matchID);
        if (debug)
        {
            cout << endl << box::disp << "DISPLAY: status" << endl;
            cout << res.body << endl;
        }
        end = std::chrono::system_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        cout << endl;
        cout << box::disp << "DISPLAY: " << "turn( " << turn << " ) (" << matchID << ")" << endl;
        cout << box::disp << "DISPLAY: " << "end( " <<  chrono::duration_cast<chrono::seconds>(end.time_since_epoch()).count() << " ) (" << matchID << ")" << endl;
        cout << box::disp << "DISPLAY: " << "処理時間( " <<  elapsed << " ) (" << matchID << ")" << endl;
        cout << box::disp << "DISPLAY: " << "かけていい時間( " <<  match.turnMillis << " ) (" << matchID << ")" << endl;
        cout << endl;

        UnixTime sec;
        
        do
        {
            struct timeval _time;
            gettimeofday(&_time, NULL);
            sec = _time.tv_sec;
        } while (sec < field.startedAtUnixTime + turn * (match.turnMillis/1000 + match.intervalMillis/1000)+1);

        cout << box::check << "GET: " << url + "matches/" << matchID << endl;
        res = GetStatus(matchID,url);
        result = CheckStatus(res);
        if (debug)
        {
            cout << endl << box::disp << "DISPLAY: status" << endl;
            cout << res.body << endl;
        }

        picojson::parse(val, res.body);
        fieldReader.setPicojsonValue(val);
        fieldReader.writeTo(field);
    }
}