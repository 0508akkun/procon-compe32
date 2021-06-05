# include <bits/stdc++.h>
# include "MatchReader.h"
# include "FieldReader.h"
# include "AccessServer.h"
# include "Battle.h"
# include "Utility/Color.h"
# include "Utility/Box.h"
# include "OptionArg.h"
using namespace std;

bool KeyWait()
{
    char keybuff[2];
    do {
        fgets(keybuff, 2, stdin);
    } while ((keybuff[0] != '\n') && (keybuff[1] != '\n'));

    return true;
}

string GetExtension(const string &path)
{
    string ext;
    size_t pos1 = path.rfind('.');
    if(pos1 != string::npos){
        ext = path.substr(pos1+1, path.size()-pos1);
        string::iterator itr = ext.begin();
        while(itr != ext.end()){
            *itr = tolower(*itr);
            itr++;
        }
        itr = ext.end()-1;
        while(itr != ext.begin()){    // パスの最後に\0やスペースがあったときの対策
            if(*itr == 0 || *itr == 32){
                ext.erase(itr--);
            }
            else{
                itr--;
            }
        }
    }
 
    return ext;
}

void RegisterOptions(int argc, char* argv[])
{
    OptionArg::Register("debug", 'd', "デバッグモードで実行します");
    OptionArg::Register("ping", 'p', "動作確認用APIを実行します");
    OptionArg::Register("local", 'l', "ローカルモードで実行します");
    OptionArg::Register<string>("input", 'i', "ローカルモードの入力ファイル名を指定します", false);
    OptionArg::Register<string>("output", 'o', "ローカルモードの出力ファイル名を指定します", false, "path/to/output");
    OptionArg::Register("random", 'r', "ローカルモードのフィールド生成をランダムで行います");
    OptionArg::Register<int>("turns", 0, "ローカルモードのターン数を指定します", false, 30);
    OptionArg::Register<int>("turnms", 0, "ローカルモードの1ターンあたりの時間を指定します", false, 5000);
    OptionArg::Register<int>("portnum",'n',"ポート番号を指定します",false,8081);
    OptionArg::Register<string>("address",'a',"IPアドレスを指定します",false,"localhost");
    OptionArg::ParseCheck(argc, argv);
}


int main(int argc, char* argv[])
{
    RegisterOptions(argc, argv);
    OptionArg::ParseCheck(argc,argv);
    bool debug = OptionArg::Exist("debug");
    bool local = OptionArg::Exist("local");
    string url = OptionArg::GetURLInfo();
    Response response;

    if (OptionArg::Exist("ping") == true)
    {
        cout << box::check << "GET: " << url << "/ping" << endl;
        response = RunPing(url);
        CheckStatus(response);
        return 0;
    }

    if (local)
    {
        Field field;
        if (OptionArg::Exist("random"))
        {
            // ランダムでフィールド生成
        }
        else
        {
            string path = OptionArg::Get<string>("input");
            std::ifstream ifs;
            ifs.open(path);
            assert(ifs);
            picojson::value val;
            string extension = GetExtension(path);
            if (extension != "json")
            {
                cout << box::failure << "ERROR: json形式のファイルを入力してください" << endl;
                return 0;
            }
            ifs >> val;
            ifs.close();
            FieldReader reader(val);
            reader.writeTo(field);
        }
        LocalBattle(field);
    }
    else // !local
    {
        cout << box::input << "INPUT: Enterキーで試合事前情報を取得します";
        KeyWait();

        cout << box::check << "GET: " << url << "/matches" << endl;

        response = GetMatches(url);
        bool result = CheckStatus(response);
        if (result == false)
        {
            return 0;
        }
        if (debug)
        {
            cout << endl << box::disp << "DISPLAY: matches" << endl;
            cout << response.body << endl;
        }

        picojson::value val;
        const string err = picojson::parse(val, response.body);
        if (err.empty() == false)
        {
            cerr << err << endl;
            return 0;
        }
        
        MatchReader matchReader = MatchReader(val);
        vector<Match> matches;
        matchReader.writeTo(matches);

        cout << box::disp << "matchID = [ ";
        for (int i = 0; i < matches.size(); ++i)
        {
            cout << matches[i].id << " ";
        }
        cout << "]" << endl;

        cout << box::input << "INPUT: match index" << endl;
        cout << box::input;
        int ID;
        cin >> ID;

        Battle(matches[ID], debug, 0);
    }
    
    return 0;
}
